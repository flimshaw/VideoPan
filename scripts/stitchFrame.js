#!/usr/bin/env node
var fs = require('fs');
var mkdirp = require('mkdirp');
var mustache = require('mustache');
var path = require('path');
var program = require('commander');
var querystring = require('querystring');
var sprintf = require('sprintf').sprintf;
var execSync = require('execSync');
var _ = require('underscore');

program
  .version('0.1.0')
  .usage('[options] [srcDir]')
  .option('-n, --frameNumber <frameNumber>', 'specify the log format string')
  .option('-t, --template <templateFile>', 'specify a pto template file', 'template.mustache')
  .parse(process.argv);

var srcDir = program.args[0] || process.exit(0)

var templateString = fs.readFileSync(program.template, 'utf8');

var Stitcher = function(templateString, frameNumber, srcDir) {
	
	var self = this;

	this.templateString = templateString;
	this.frameNumber = frameNumber;
	this.srcDir = srcDir;
	this.destDir = path.join(srcDir, 'render', this.frameNumber.toString());

	// create destination directory
	mkdirp.sync(this.destDir);

	// generate a pto file for this directory
	this.ptoFilePath = this.generatePtoFile();

	// run nona
	console.log("Remapping...");
	var nonaCmd = sprintf("nona -z NONE -r ldr -m TIFF_m -o %s/ -v %s", this.destDir, this.ptoFilePath);
	var nona = execSync.code(nonaCmd);

	// run enblend
	console.log("Enblending...");
	var enblendCmd = sprintf("enblend -l 9 -v -w -o %s", path.join(this.destDir, this.frameNumber + "_rendered.jpg"));
	_.each([0, 1, 2], function(file) {
		enblendCmd += sprintf(" %s/%04d.tif", self.destDir, file);
	});
	var enblend = execSync.code(enblendCmd);

}

Stitcher.prototype.renderFrame = function(frameNumber) {
	this.generatePtoFile(frameNumber);

}

Stitcher.prototype.getDestDir = function(frameNumber) {
	return path.join(this.srcDir, 'render', frameNumber.toString());
}

Stitcher.prototype.getSrcFilePath = function() {
	var group = Math.floor((this.frameNumber - 1) / 1000);
	var srcFilePath = path.join(group.toString(), sprintf("%05d.jpg", Number(this.frameNumber)));
	return srcFilePath;
}

Stitcher.prototype.generatePtoFile = function(frameNumber) {
	var view = {
		frameNumber: this.frameNumber,
		srcDir: this.srcDir,
		srcFilePath: this.getSrcFilePath(),
		destDir: this.destDir,
		destFile: this.frameNumber + ".pto"
	}

	// create a folder for this frame number
	mkdirp.sync(view.destDir);

	var ptoFile = mustache.render(this.templateString, view);
	var ptoFilePath = path.join(view.destDir, view.destFile);
	fs.writeFileSync(ptoFilePath, ptoFile);
	return ptoFilePath;
}

var stitcher = new Stitcher(templateString, program.frameNumber, srcDir);