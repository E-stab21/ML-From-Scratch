import express from 'express'
import path from 'path';
import execFile from 'child_process'

//globals
const port = 3000;
const exePath = 'C:\\'

//sets up server
const app = express();

//middleware
app.use(express.json());
app.use(express.static(path.join(import.meta.dirname, 'public')));

app.get('*', (req, res) => {
    res.sendFile(path.join(import.meta.dirname, 'public', 'index.html'));
});

app.post('/data', (req, res) => {
    const args = Object.values(req.body);

    execFile(exePath, args, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing file: ${error.message}`);
            return;
        }
        if (stderr) {
            console.error(`Standard Error: ${stderr}`);
            return;
        }
        console.log(`Standard Output: ${stdout}`);
    });


})

app.listen(port, () => {
    console.log(`listening on port ${port}`);
});

