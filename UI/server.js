import express from 'express'
import path from 'path';
import { spawn } from 'child_process'

//globals
const port = 3000;
const exePath = path.join(import.meta.dirname, '..', 'cmake-build-debug', 'ML-From-Scratch.exe');
const exeDir = path.join(import.meta.dirname, '..', 'cmake-build-debug');

//sets up server
const app = express();

//middleware
app.use(express.json());
app.use(express.static(path.join(import.meta.dirname, 'public')));

app.post('/data', (req, res) => {
    const args = Object.values(req.body);

    //checking args
    console.log(args);

    let stderrOutput = "";
    const child = spawn(exePath, args, {
        cwd: exeDir
    });

    //on start fail
    child.on('error', (err) => {
        console.error('Failed to start C++ process:', err);
        res.status(500).send('Data Generation failed');
    });

    //on in process error
    child.stderr.on('data', (data) => {
        stderrOutput += data.toString();
    });

    //on close
    child.on('close', (code) => {
        if (code !== 0) {
            console.error(`C++ exited with code ${code}: ${stderrOutput}`);
            if (!res.headersSent) {
                res.status(500).send('Data Generation failed');
            }
        }
        res.end();
    });

    res.setHeader('Content-Type', 'text/csv');

    child.stdout.pipe(res);
});

app.get('*', (req, res) => {
    res.sendFile(path.join(import.meta.dirname, 'public', 'index.html'));
});

app.listen(port, () => {
    console.log(`listening on port ${port}`);
});

