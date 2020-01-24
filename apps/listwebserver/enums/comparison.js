/**
 * Enums for Comparison types
 */

const deepFreeze = require('deep-freeze');

const COMPARISON_TYPES = { // must be the name of an analyzer, e.g. apps/listwebserver/analyzers/psnrAndDelay.js
    PSNR_AND_DELAY: 'psnrAndDelay',
};

module.exports = deepFreeze(COMPARISON_TYPES);
