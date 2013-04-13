#!/usr/bin/env node
var path = require('path')
var mkdirp = require('mkdirp')
var ffmpeg = require('ffmpeg');

// the video file is the first argument
var videoFile = process.argv[2];

// create a directory for the videos in a folder named the file name, ie REAR/
var videoDir = path.basename(videoFile, path.extname(videoFile));

// create our video directory, or exit with an error
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

// TODO: Verify the video file is a video file

try {
    new ffmpeg(videoFile, function (err, video) {
        if (!err) {
            processVideoFile(video);
        } else {
            console.log('Error: ' + err);
        }
    });
} catch (e) {
    console.log(e.code);
    console.log(e.msg);
}

function processVideoFile(video) {
	console.log(video);
}

ffmpeg(videoFile);