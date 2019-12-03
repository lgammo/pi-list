# Video Parameters Explained

## SMPTE ST 2110-21

### Basic definitions

| Parameter | Explanation |
| ------ | ------ |
| T<sub>frame</sub> | Frame/field period |
| N | Frame or field index since the SMPTE epoch |
| N x T<sub>frame</sub> | Frame/field reference time |
| TP<i>R</i><sub>j</sub> | Ideal time when a packet j gets out off the receive buffer in the Read Schedule model |
| T<i>VD</i>) |  Video Transmission Datum: TP<sub>R</sub><sub>0</sub> = T<subVD</sub> |
| TP<i>A</i><sub>j</sub> | Actual measured arrival time of a packet |

![Packet Arrival](video_timings.png)

### Results

| Parameter | Explanation |
| ------ | ------ |
| C<sub>INST</sub>  | Instantaneous value of the *Retwork Compatibility model* C as defined in SMPTE ST 2110-21 |
Base definition:
| C<sub>MAX</sub> | Maximum allowed value that C<sub>INST</sub> shall not exceed. This value is dependend on the type of sender, the video format and the packing of the video. A narrow sender has a tighter packet pacing on the network and will have a lower allowed C<sub>MAX</sub> value than a wide sender. Both sender type are limited in their burstiness. |
| VRX | Measured level of the *Virtual Receive Buffer* as defined in SMPTE ST 2110-21 |
| VRX<sub>FULL</sub> | Maximum allowed value the VRX buffer. This value is dependend on the type of sender, the video format and the packing of the video. A narrow sender has a smaller receiving buffer and will have a lower allowed VRX<sub>Full</sub> value than a wide sender. Both sender type are limited in their burstiness. |
| FPO | First Packet Offset measured between frame/field reference time and the first *captured* packet of a field/frame. This is a like a measured TR<sub>offset</sub> supposedly close to TRO<sub>default</sub>. In practise this value will be a bit smaller, as a result of this difference some packets will be sinked into the VRX buffer. |
| TRO<sub>default</sub> | Default *theoretical* offset between the frame/field reference time and the first packet of a field or frame to be *read*. |

## Analysis method

### Requirements

- *Accuracy*: The clock of the capture device must be locked to the same PTP master as the sender device. Experience has often shown the opposite. We use algorithms that try to minimize the impact of a non-locked capture or sender device. However, the results are not guaranteed to be accurate.
- *Precision*: The capture must be done using nanosecond resolution pcap files.

### Assumptions

- *Schedule*: LIST does not rely on SDP files, therefore it is doesn't know if the schedule is gapped or linear. Therefore, we use an heuristic to determine that. That heuristic is based on the gap between the capture time of the last packet of one frame and the first packet of the subsequent frame. If that gap is at least 10 times the inter packet spacing of all other packets, we assume the schedule is gapped.
- *VRX buffer* should be empty at the begin of each frame. This is not necessarily correct and we may eventually remove this constraint. (see https://github.com/ebu/pi-list/issues/18)
- Packets are processed in their *arrival order* for C<sub>INT</sub> and VRX. Packet shuffling (RTP sequence numbers) is ignored.

### VRX

Since we don't read SDP files, we are not aware of any announced TR<sub>offset</sub>. So we use 2 different algorithms to calculate VRX:

- *Ideal*:  TR<sub>offset</sub> = TRO<sub>default</sub> = T<sub>VD</sub> - N x T<sub>frame</sub>
- *Measured*: TR<sub>offset</sub> = FPO = avg(TP<i>A</i><sub>0</sub> - N x T<sub>frame</sub>)

## RTP Timestamps

| Parameter | Explanation |
| ------ | ------ |
| Delta 1st Packet vs RTP | TP<i>A</i><sub>0</sub> - RTP<sub>Timestamp</sub>. Acceptable range is given by [JT-NM Tested Program](http://jt-nm.org/documents/JT-NM_Tested_Catalog_ST2110_Full-Online-2019-09-10.pdf) Section 5.3: "The instantaneous value of the RTP timestamp of the stream is not in the future, not more than 1 ms in the past (unless justified), and preserve a stable relation to the PTP" |
| Delta RTP vs Reference Frame | RTP<sub>Timestamp</sub> - N x T<sub>frame</sub>. This quantifies the precision of RTP<sub>Timestamp</sub> assuming that the packets arrives between N x T<sub>frame</sub> and N+1 x T<sub>frame</sub>. Range [-90,0] ticks (1ms) is arbitrary for now. |