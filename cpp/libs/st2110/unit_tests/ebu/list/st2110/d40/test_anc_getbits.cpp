#include "catch2/catch.hpp"
#include "ebu/list/st2110/d40/packet.h"
#include "utils.h"

using namespace ebu_list;
using namespace ebu_list::st2110;
using namespace ebu_list::st2110::d40;

//------------------------------------------------------------------------------

SCENARIO("Read n-bit words in byte-unaligned buffer")
{
    GIVEN("8-bit aligned words")
    {
        auto buffer          = oview(make_static_sbuffer(to_byte_array(0b00000000, 0b11111111, 0b10101010)));
        auto p               = buffer.view().data();
        uint16_t bit_counter = 0;
        WHEN("we decode")
        {
            THEN("we get the correct values")
            {
                REQUIRE(get_bits<8>(&p, &bit_counter) == 0b00000000);
                REQUIRE(get_bits<8>(&p, &bit_counter) == 0b11111111);
                REQUIRE(get_bits<8>(&p, &bit_counter) == 0b10101010);
            }
        }
    }

    GIVEN("16-bit aligned words")
    {
        auto buffer = oview(
            make_static_sbuffer(to_byte_array(0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b10101010, 0b10101010)));
        auto p               = buffer.view().data();
        uint16_t bit_counter = 0;

        WHEN("we decode")
        {
            THEN("we get the correct values")
            {
                REQUIRE(get_bits<16>(&p, &bit_counter) == 0b0000000000000000);
                REQUIRE(get_bits<16>(&p, &bit_counter) == 0b1111111111111111);
                REQUIRE(get_bits<16>(&p, &bit_counter) == 0b1010101010101010);
            }
        }
    }

    GIVEN("multiple words in a byte")
    {
        auto buffer          = oview(make_static_sbuffer(to_byte_array(0b11110011, 0b00011100, 0b10101010)));
        auto p               = buffer.view().data();
        uint16_t bit_counter = 0;

        WHEN("we decode")
        {
            THEN("we get the correct values")
            {
                REQUIRE(get_bits<4>(&p, &bit_counter) == 0b1111);
                REQUIRE(get_bits<2>(&p, &bit_counter) == 0b00);
                REQUIRE(get_bits<2>(&p, &bit_counter) == 0b11);

                REQUIRE(get_bits<3>(&p, &bit_counter) == 0b000);
                REQUIRE(get_bits<3>(&p, &bit_counter) == 0b111);
                REQUIRE(get_bits<2>(&p, &bit_counter) == 0b00);

                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b1);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b0);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b1);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b0);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b1);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b0);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b1);
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b0);
            }
        }
    }

    GIVEN("multiple words in multiple bytes")
    {
        auto buffer = oview(
            make_static_sbuffer(to_byte_array(0b11110000, 0b00001111, 0b11111111, 0b00111111, 0b11111111, 0b11000000)));
        auto p               = buffer.view().data();
        uint16_t bit_counter = 0;

        WHEN("we decode")
        {
            THEN("we get the correct values")
            {
                REQUIRE(get_bits<4>(&p, &bit_counter) == 0b1111);
                REQUIRE(get_bits<8>(&p, &bit_counter) == 0b00000000);
                REQUIRE(get_bits<12>(&p, &bit_counter) == 0b111111111111);

                REQUIRE(get_bits<2>(&p, &bit_counter) == 0b00);
                REQUIRE(get_bits<16>(&p, &bit_counter) == 0b1111111111111111);
                REQUIRE(get_bits<6>(&p, &bit_counter) == 0b000000);
            }
        }
    }

    /* ancillary data is mostly made of 10-bit words */
    GIVEN("10-bit words even-aligned")
    {
        auto buffer =
            oview(make_static_sbuffer(to_byte_array(0b11111111, 0b11000000, 0b00001111, 0b11111100, 0b00000000,
                                                    0b11111111, 0b11000000, 0b00001111, 0b11111100, 0b00000000)));
        auto p               = buffer.view().data();
        uint16_t bit_counter = 0;

        WHEN("we decode")
        {
            THEN("we get the correct values")
            {
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
            }
        }
    }

    GIVEN("10-bit words odd-aligned")
    {
        auto buffer =
            oview(make_static_sbuffer(to_byte_array(0b01111111, 0b11100000, 0b00000111, 0b11111110, 0b00000000,
                                                    0b01111111, 0b11100000, 0b00000111, 0b11111110, 0b00000000)));
        auto p               = buffer.view().data();
        uint16_t bit_counter = 0;

        WHEN("we decode")
        {
            THEN("we get the correct values")
            {
                REQUIRE(get_bits<1>(&p, &bit_counter) == 0b0);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b0000000000);
                REQUIRE(get_bits<10>(&p, &bit_counter) == 0b1111111111);
                REQUIRE(get_bits<9>(&p, &bit_counter) == 0b000000000);
            }
        }
    }
}
