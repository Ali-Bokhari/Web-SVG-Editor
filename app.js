'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  if (typeof req.files.uploadFile == 'undefined') {
    res.redirect('/');
    return;
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ********************

let parser = ffi.Library('./libsvgparse', {
  'createValidSVGimage': [ 'pointer', ['string', 'string'] ],
  'SVGtoJSON': [ 'string', [ 'pointer' ] ],
  'deleteSVGimage' : [ 'void', ['pointer'] ],
  'getTitle' : ['string', ['pointer']],
  'getDesc' : ['string', ['pointer']],
  'getRectListJSON' : ['string', ['pointer']],
  'getCircListJSON' : ['string', ['pointer']],
  'getPathListJSON' : ['string', ['pointer']],
  'getGroupListJSON' : ['string', ['pointer']],
  'makeAttr' : ['pointer', ['string', 'string']],
  'setAttribute' : ['void', ['pointer', 'int', 'int', 'pointer']],
  'validateSVGimage' : ['int', ['pointer', 'string']],
  'writeSVGimage' : ['int', ['pointer', 'string']],
  'setTitle' : ['void', ['pointer', 'string']],
  'setDescription' : ['void', ['pointer', 'string']],
  'JSONtoSVG' : ['pointer',['string']],
  'JSONtoRect' : ['pointer', ['string']],
  'JSONtoCircle' : ['pointer', ['string']],
  'createSVG' : ['pointer', []],
  'addComponent' : ['void',['pointer', 'int', 'pointer']],
  'scaleRects' : ['void', ['pointer', 'float']],
  'scaleCircs' : ['void', ['pointer', 'float']]
});

app.get('/images', function(req , res){
  var dict = {};
  var files = fs.readdirSync(__dirname+'/uploads/');
  for (const file of files) {
    let c = parser.createValidSVGimage(__dirname+"/uploads/" + file, __dirname + "/parser/svg.xsd");
    let s = parser.SVGtoJSON(c);
    if (s != "{}") {
      const stats = fs.statSync("uploads/" + file);
      const fileSizeInBytes = stats.size;
      dict[file] = JSON.parse(s);
      dict[file]["size"] = Math.round(fileSizeInBytes / 1000);
    }
    parser.deleteSVGimage(c);
  }
  res.send(dict);
});

app.get('/viewpanel', function(req, res) {
  console.log(req.query.file);
  var image = {};
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.file, __dirname + "/parser/svg.xsd");
  let c = parser.getTitle(s);
  image["title"] = c;
  c = parser.getDesc(s);
  image["description"] = c;
  c = parser.getRectListJSON(s);
  image["rectangles"] = JSON.parse(c);
  c = parser.getCircListJSON(s);
  image["circles"] = JSON.parse(c);
  c = parser.getPathListJSON(s);
  image["paths"] = JSON.parse(c);
  c = parser.getGroupListJSON(s);
  image["groups"] = JSON.parse(c);
  parser.deleteSVGimage(s);
  res.send(image);
});

app.get('/edit', function(req, res) {
  console.log(req.query);
  let shape = req.query.shape;
  let shapen = -1;
  if (shape == 'SvgImage') {
    shapen = 0;
  } else if (shape == 'Circle') {
    shapen = 1;
  } else if (shape == 'Rectangle') {
    shapen = 2;
  } else if (shape == 'Path') {
    shapen = 3;
  } else if (shape == 'Group') {
    shapen = 4;
  }
  let img = parser.createValidSVGimage(__dirname + "/uploads/" + req.query.image, __dirname + "/parser/svg.xsd");
  let attr = parser.makeAttr(req.query.name, req.query.value);
  parser.setAttribute(img, shapen, parseInt(req.query.index), attr);
  let valid = parser.validateSVGimage(img, __dirname + "/parser/svg.xsd");
  if (valid == 1) {
    let saved = parser.writeSVGimage(img, __dirname + "/uploads/" + req.query.image);
    parser.writeSVGimage(img, __dirname + "/uploads/" + req.query.image);
    parser.deleteSVGimage(img);
    if (saved == 1) {
      res.send({
        status : 1
      });
    } else {
      res.send({
        status: 0
      });
    }
  } else{
    parser.deleteSVGimage(img);
    res.send ({
      status : 0
    });
  }
});

app.get('/edit-title', function(req, res) {
  console.log(req.query.title);
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.image, __dirname + "/parser/svg.xsd");
  parser.setTitle(s, req.query.title);
  let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.image);
  if (saved == 1) {
    res.send ({
      status : 1
    });
  } else {
    res.send ({
      status : 0
    });
  }
  parser.deleteSVGimage(s);
});

app.get('/edit-description', function(req, res) {
  console.log(req.query.description);
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.image, __dirname + "/parser/svg.xsd");
  parser.setDescription(s, req.query.description);
  let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.image);
  if (saved == 1) {
    res.send ({
      status : 1
    });
  } else {
    res.send ({
      status : 0
    });
  }
  parser.deleteSVGimage(s);
});

app.get('/create-file', function(req, res) {
  console.log(req.query.filename);
  var files = fs.readdirSync(__dirname+'/uploads/');
  if(files.includes(req.query.filename)) {
    res.send({
      status: 0
    });
    return;
  }
  let splited = req.query.filename.split(".");
  if (splited[1] != 'svg') {
    res.send({
      status: 0
    });
    return;
  }
  let s = parser.createSVG();
  let valid = parser.validateSVGimage(s, __dirname + "/parser/svg.xsd");
  if (valid == 1) {
    let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.filename);
    parser.deleteSVGimage(s);
    if (saved == 1) {
      res.send({
        status : 1
      });
    } else {
      res.send({
        status: 0
      });
    }
  } else{
    parser.deleteSVGimage(s);
    res.send ({
      status : 0
    });
  }
});

app.get('/add-rect', function(req, res) {
  console.log(req.query.filename);
  let rect = {
    x: parseFloat(req.query.x),
    y: parseFloat(req.query.y),
    w: parseFloat(req.query.w),
    h: parseFloat(req.query.h),
    units: req.query.units
  }
  console.log(JSON.stringify(rect));
  let srect = parser.JSONtoRect(JSON.stringify(rect));
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.filename, __dirname + "/parser/svg.xsd");
  //let srect = parser.createRect(parseFloat(req.query.x), parseFloat(req.query.y), parseFloat(req.query.w), parseFloat(req.query.h), req.query.units);
  //console.log(parser.rectangleToString(srect));
  parser.addComponent(s, 2, srect);
  let valid = parser.validateSVGimage(s, __dirname + "/parser/svg.xsd");
  if (valid == 1) {
    let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.filename);
    parser.deleteSVGimage(s);
    if (saved == 1) {
      res.send({
        status : 1
      });
    } else {
      res.send({
        status: 0
      });
    }
  } else{
    parser.deleteSVGimage(s);
    res.send ({
      status : 0
    });
  }
});

app.get('/add-circ', function(req, res) {
  console.log(req.query.filename);
  let circ = {
    cx: parseFloat(req.query.x),
    cy: parseFloat(req.query.y),
    r: parseFloat(req.query.r),
    units: req.query.units
  }
  console.log(JSON.stringify(circ));
  let srect = parser.JSONtoCircle(JSON.stringify(circ));
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.filename, __dirname + "/parser/svg.xsd");
  //let srect = parser.createRect(parseFloat(req.query.x), parseFloat(req.query.y), parseFloat(req.query.w), parseFloat(req.query.h), req.query.units);
  //console.log(parser.rectangleToString(srect));
  parser.addComponent(s, 1, srect);
  let valid = parser.validateSVGimage(s, __dirname + "/parser/svg.xsd");
  if (valid == 1) {
    let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.filename);
    parser.deleteSVGimage(s);
    if (saved == 1) {
      res.send({
        status : 1
      });
    } else {
      res.send({
        status: 0
      });
    }
  } else{
    parser.deleteSVGimage(s);
    res.send ({
      status : 0
    });
  }
});

app.get('/scale-rects', function(req, res) {
  console.log(req.query.filename);
  //let srect = parser.JSONtoCircle(JSON.stringify(circ));
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.filename, __dirname + "/parser/svg.xsd");
  parser.scaleRects(s, parseFloat(req.query.scale));
  //let srect = parser.createRect(parseFloat(req.query.x), parseFloat(req.query.y), parseFloat(req.query.w), parseFloat(req.query.h), req.query.units);
  //console.log(parser.rectangleToString(srect));
  //parser.addComponent(s, 1, srect);
  let valid = parser.validateSVGimage(s, __dirname + "/parser/svg.xsd");
  if (valid == 1) {
    let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.filename);
    parser.deleteSVGimage(s);
    if (saved == 1) {
      res.send({
        status : 1
      });
    } else {
      res.send({
        status: 0
      });
    }
  } else{
    parser.deleteSVGimage(s);
    res.send ({
      status : 0
    });
  }
});

app.get('/scale-circs', function(req, res) {
  console.log(req.query.filename);
  //let srect = parser.JSONtoCircle(JSON.stringify(circ));
  let s = parser.createValidSVGimage(__dirname+"/uploads/" + req.query.filename, __dirname + "/parser/svg.xsd");
  parser.scaleCircs(s, parseFloat(req.query.scale));
  //let srect = parser.createRect(parseFloat(req.query.x), parseFloat(req.query.y), parseFloat(req.query.w), parseFloat(req.query.h), req.query.units);
  //console.log(parser.rectangleToString(srect));
  //parser.addComponent(s, 1, srect);
  let valid = parser.validateSVGimage(s, __dirname + "/parser/svg.xsd");
  if (valid == 1) {
    let saved = parser.writeSVGimage(s, __dirname + "/uploads/" + req.query.filename);
    parser.deleteSVGimage(s);
    if (saved == 1) {
      res.send({
        status : 1
      });
    } else {
      res.send({
        status: 0
      });
    }
  } else{
    parser.deleteSVGimage(s);
    res.send ({
      status : 0
    });
  }
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
