#!/usr/bin/env node
var fs = require('fs');
var mkdirp = require('mkdirp');
var mustache = require('mustache');

// get the frame number
var frameNumber = process.argv[2];

// src directory
var srcDir = process.argv[3]

// create a folder for our renderings
mkdirp.sync(srcDir + "/render");

// generate a pto file 