#!/usr/bin/env node
var fs = require('fs');
var mkdirp = require('mkdirp');
var mustache = require('mustache');
var program = require('commander');

program
  .version('0.1.0')
  .usage('[options] [srcDir]')
  .option('-n, --frameNumber <frameNumber>', 'specify the log format string')
  .parse(process.argv);

var srcDir = program.args[0] || process.exit(0)

