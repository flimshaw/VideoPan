#!/usr/bin/env node
var fs = require('fs');
var mkdirp = require('mkdirp');
var mustache = require('mustache');
var path = require('path');
var program = require('commander');

program
  .version('0.1.0')
  .usage('[options] [srcDir]')
  .option('-n, --frameNumber <frameNumber>', 'specify the log format string')
  .option('-t, --template <templateFile>', 'specify a pto template file', 'template.mustache')
  .parse(process.argv);

var srcDir = program.args[0] || process.exit(0)

var templateString = fs.readFileSync(program.template, 'utf8');

var Stitcher = function(templateString, frameNumber, srcDir) {
	
	this.templateString = templateString;
	this.frameNumber = frameNumber;
	this.srcDir = srcDir;
	this.destDir = path.join(srcDir, 'render');

	// create destination directory
	mkdirp.sync(this.destDir);

	// generate a pto file for this directory
	this.generatePtoFile();

	console.log(this.getSrcPath());

	console.log("stitcher started");
	console.log(this.ptoFile);
}

Stitcher.prototype.getSrcFilePath = function(camera) {
	var group = Math.floor((this.frameNumber - 1) / 1000);
	var srcFilePath = path.join(group.toString(), this.frameNumber + '.jpg');
	return srcFilePath;
}

Stitcher.prototype.generatePtoFile = function() {
	var view = {
		frameNumber: this.frameNumber,
		srcDir: this.srcDir,
		destDir: path.join(this.destDir, this.frameNumber.toString()),
		destFile: this.frameNumber + ".pto"
	}

	// create a folder for this frame number
	mkdirp.sync(view.destDir);

	var ptoFile = mustache.render(this.templateString, view);
	fs.writeFileSync(path.join(view.destDir, view.destFile), ptoFile);
}

var stitcher = new Stitcher(templateString, program.frameNumber, srcDir);