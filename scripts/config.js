const fs = require('fs');

fs.copyFileSync(`${__dirname}/../config.json.example`, `${__dirname}/../config.json`);
