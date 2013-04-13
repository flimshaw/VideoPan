#!/usr/bin/env node
var path = require('path');
var mkdirp = require('mkdirp');
var sprintf = require('sprintf');
var mv = require('mv');

// how many items do we want to put into each group
var groupCount = 1000;

// current directory
var group = 1;

// make sure we have a subfolder for this group
mkdirp(group.toString(), function(e) {
	if(e) {
		switch(e) {
			case 'EEXIST':
				console.log("File with same name exists");
				break;
			default:
				console.log("unknown error: " + e);
				break;
		}
		process.exit(1);
	} else {
		processDirectory(group);
	}
});

function processDirectory(group) {

	// always start on frame 1
	var startFrame = (group * groupCount) + 1;

	// and end 1000 frames later
	var endFrame = startFrame + (groupCount - 1);

	for(var f = startFrame; f <= endFrame; f++ ) {
		console.log(sprintf.sprintf("%05d", f));
	}

}