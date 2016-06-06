'use strict';

const Buffer = require('buffer').Buffer;
const Transform = require('_stream_transform');
const binding = process.binding('ThostFtdcMdApi');
const util = require('util');
const assert = require('assert').ok;
const kMaxLength = require('buffer').kMaxLength;
const kRangeErrorMessage = 'Cannot create final Buffer. ' +
    'It would be larger than 0x' + kMaxLength.toString(16) + ' bytes.';

const ThostFtdcMdApi = binding.ThostFtdcMdApi;

exports.createThostFtdcMdApi = function(o) {
    return new ThostFtdcMdApi(o);
}

