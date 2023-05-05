const express = require("express");
const readline = require("readline");
const fs = require("fs");

async function processLineByLine(filepath) {
  const fileStream = fs.createReadStream(filepath);

  const rl = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity,
  });

  for await (const line of rl) {
    console.log(`Line: ${line}`);
  }
}

const app = express();

app.get('/', (req, res) => {
  processLineByLine('file.txt');
  res.sendFile('./index.html', {root: __dirname});
})

app.listen(3000);
