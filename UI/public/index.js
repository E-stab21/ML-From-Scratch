const form = document.getElementById('params');
let lossChart = null;

form.addEventListener('submit', async () => {
    //stop the page from refreshing
    event.preventDefault();

    //create the data object to send
    const formData = {
        number_of_layers: document.getElementById('number_of_layers').value,
        middle_layer_size: document.getElementById('middle_layer_size').value,
        training_size: document.getElementById('training_size').value,
        batch_size: document.getElementById('batch_size').value,
        epochs: document.getElementById('epochs').value,
        min_lr: document.getElementById('min_lr').value,
        max_lr: document.getElementById('max_lr').value,
        beta: document.getElementById('beta').value
    };

    //grab data
    const lossData = await getData(formData);
    if (!lossData) {
        return;
    }

    //format data
    const percentCorrect = parseFloat(lossData[0][0]);
    const labels = [];
    const values = [];
    lossData.slice(1).forEach(row => {
        labels.push(parseFloat(row[0]));
        values.push(parseFloat(row[1]))
    })

    document.getElementById("accuracy").textContent = percentCorrect + '%';

    //reset graph
    if (lossChart !== null) {
        lossChart.destroy();
    }

    //make the graph
    const ctx = document.getElementById('lossChart').getContext('2d');
    lossChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'Model Loss',
                data: values,
                borderColor: 'rgb(75, 192, 192)',
                tension: 0.1
            }]
        },
        options: {
            scales: {
                y: { beginAtZero: true }
            }
        }
    });
});

 async function getData(formData) {
    try {
        //send it to the server
        const response = await fetch('http://localhost:3000/data', {
            method: 'POST', //use POST to send data
            headers: {
                'Content-Type': 'application/json' //tell the server we are sending JSON
            },
            body: JSON.stringify(formData) //turn the JS object into a string
        });

        if (!response.ok) {
            console.error(`Server responded with status: ${response.status}`);
            return;
        }

        const csvData = await response.text();
        if (!csvData) {
            console.error('Data stream ended');
            return;
        }

        return csvData.split('\n').map(row => row.split(','));

    } catch (error) {
        console.error('network failure or lost connection');
    }
}

