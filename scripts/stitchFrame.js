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
var sleep = require('sleep');

program
  .version('0.1.0')
  .usage('[options] [srcDir]')
  .option('-n, --startFrame <startFrame>', 'specify the frame to start on')
  .option('-c, --count <count>', 'specify how many frames to render', 1)
  .option('-t, --template <templateFile>', 'specify a pto template file', 'template.mustache')
  .parse(process.argv);

var srcDir = program.args[0] || process.exit(0)

var templateString = fs.readFileSync(program.template, 'utf8');


var Stitcher = function(templateString, startFrame, count, srcDir) {
	
	var self = this;

	this.templateString = templateString;
	this.startFrame = Number(startFrame);
	this.count = Number(count);
	this.srcDir = srcDir;
	this.destDir = path.join(srcDir, 'render');

	if(count) {
		for(var i = this.startFrame; i < this.startFrame + this.count; i++) {
			this.renderFrame(i);
		}
	} else {
		this.renderFrame(this.startFrame);
	}
}

Stitcher.prototype.renderFrame = function(frameNumber) {
	
	var self = this;

	// create destination directory
	mkdirp.sync(this.getDestDir(frameNumber));

	// make a pto file
	var ptoFilePath = this.generatePtoFile(frameNumber);

	// run nona
	console.log(sprintf("Remapping frame %05d ...", frameNumber));
	var nonaCmd = sprintf("nona -z NONE -r ldr -m TIFF_m -o %s/ -v %s", this.getDestDir(frameNumber), ptoFilePath);
	var nona = execSync.code(nonaCmd);
	
	// run enblend
	console.log(sprintf("Enblending frame %05d ...", frameNumber));
	var enblendCmd = sprintf("enblend -l 9 -v -w -o %s", this.getDestFilePath(frameNumber));
	_.each([0, 1, 2], function(file) {
		enblendCmd += sprintf(" %s/%04d.tif", self.getDestDir(frameNumber), file);
	});
	var enblend = execSync.code(enblendCmd);

	// remove tif files
	_.each([0, 1, 2], function(file) {
		fs.unlink(sprintf("%s/%04d.tif", self.getDestDir(frameNumber), file));
	});

	// remove pto file
	fs.unlink(ptoFilePath);

	// move the final file to the appropriate directory
	this.submitFile(frameNumber);

	this.cleanup();

}

Stitcher.prototype.cleanup = function() {
	execSync.code("rmdir " + this.destDir + "/temp*");
}

// submit our file
Stitcher.prototype.submitFile = function(frameNumber) {
	var self = this;
	mkdirp.sync(this.getFinalFileDir());
	fs.renameSync(this.getDestFilePath(frameNumber), this.getFinalFilePath(frameNumber));
}

Stitcher.prototype.getDestDir = function(frameNumber) {
	return path.join(this.srcDir, 'render', 'temp_' + frameNumber.toString());
}

Stitcher.prototype.getDestFilePath = function(frameNumber) {
	return path.join(this.getDestDir(frameNumber), frameNumber + "_rendered.jpg");
}

Stitcher.prototype.getFinalFilePath = function(frameNumber) {
	return path.join(this.getFinalFileDir(frameNumber), sprintf("%05d.jpg", frameNumber));
}

Stitcher.prototype.getFinalFileDir = function(frameNumber) {
	var destGroup = Math.floor((frameNumber - 1) / 1000) || 0;
	return path.join(this.destDir, destGroup.toString());
}

Stitcher.prototype.getSrcFilePath = function(frameNumber) {
	var group = Math.floor((frameNumber - 1) / 1000);
	var srcFilePath = path.join(group.toString(), sprintf("%05d.jpg", Number(frameNumber)));
	return srcFilePath;
}

Stitcher.prototype.generatePtoFile = function(frameNumber) {
	var view = {
		frameNumber: this.frameNumber,
		srcDir: this.srcDir,
		srcFilePath: this.getSrcFilePath(frameNumber),
		destDir: this.getDestDir(frameNumber),
		destFile: frameNumber + ".pto"
	}

	// create a folder for this frame number
	mkdirp.sync(view.destDir);

	var ptoFile = mustache.render(this.templateString, view);
	var ptoFilePath = path.join(view.destDir, view.destFile);
	fs.writeFileSync(ptoFilePath, ptoFile);
	return ptoFilePath;
}

var stitcher = new Stitcher(templateString, program.startFrame, program.count, srcDir);