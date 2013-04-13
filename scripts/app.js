#!/usr/bin/env node
var path = require('path')
var mkdirp = require('mkdirp')

// the video file is the first argument
var videoFile = process.argv[2];

// create a directory for the videos in a folder named the file name, ie REAR/
var videoDir = path.basename(videoFile, path.extname(videoFile));

mkdirp(videoDir, function(e) {
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
		console.log("Directory verified.");
	}
});