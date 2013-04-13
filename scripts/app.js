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
    var vproc = new ffmpeg(process.env.PWD + '/' + videoFile);
    vproc.then(function (video) {
        // Callback mode
        video.addCommand('-q', '2');
        video.fnExtractFrameToJPG(videoDir, {
            
        }, function (error, files) {
            if (!error)
                console.log('Frames: ' + files);
            else
            	console.log(error)
        });
    }, function (err) {
        console.log('Error: ' + err);
    });
} catch (e) {
    console.log(e.code);
    console.log(e.msg);
}