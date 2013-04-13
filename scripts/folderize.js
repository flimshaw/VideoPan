#!/usr/bin/env node
var path = require('path');
var mkdirp = require('mkdirp');
var sprintf = require('sprintf');
var fs = require('fs');

// get the starting directory
var rootDir = process.argv[2];

// how many items do we want to put into each group
var groupCount = 1000;


function processFile(fileName, group, endFrame) {
	return function(err) {
		if(err) {
			console.log(fileName + " not found.");
			process.exit(0);
		} else {
			console.log(fileName + " moved.");
		}
	}
}

function processDirectory(group) {

	console.log("Processing group " + group + "...");

	// always start on frame 1
	var startFrame = (group * groupCount) + 1;

	// and end 1000 frames later
	var endFrame = startFrame + (groupCount - 1);

	for(var f = startFrame; f <= endFrame; f++ ) {
		var fileName = sprintf.sprintf("%05d.jpg", f);
		var srcFilePath = path.normalize(rootDir + "/" + fileName);
		var destFilePath = path.normalize(rootDir + "/" + group + "/" + fileName);
		try {
			fs.renameSync(srcFilePath, destFilePath);
		} catch(e) {
			if(e.code == 'ENOENT') {
				console.log("Couldn't move " + e.path);
				process.exit(1);
			} else {
				console.log(e);
			}
		}
		
	}

}

for(var group = 0; group < 1000; group++) {
	// make sure we have a subfolder for this group
	var newDir = path.normalize(rootDir + "/" + group.toString());
	mkdirp.sync(newDir);
	processDirectory(group);
}
