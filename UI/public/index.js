const form = document.getElementById('params');

myForm.addEventListener('submit', () => {
    // 1. Stop the page from refreshing
    event.preventDefault();

    //const formData = new FormData(myForm);
    //const data = Object.fromEntries(formData); // Converts all inputs into one object automatically


    // 3. Create the data object to send
    const data = {
        number_of_layers: document.getElementById('number_of_layers').value,
        middle_layer_size: document.getElementById('middle_layer_size').value,
        training_size: document.getElementById('training_size').value,
        batch_size: document.getElementById('batch_size').value,
        epochs: document.getElementById('epochs').value,
        min_lr: document.getElementById('min_lr').value,
        max_lr: document.getElementById('max_lr').value,
        beta: document.getElementById('beta').value
    };

    // 4. Send it to the server
    fetch('https://localhost:3000/data', {
        method: 'POST', // Use POST to send data
        headers: {
            'Content-Type': 'application/json' // Tell the server we are sending JSON
        },
        body: JSON.stringify(data) // Turn the JS object into a string
    })
        .then(response => response.json()) // Wait for server to say "Got it!"
        .then(result => {
            console.log('Success:', result);
            alert('Data sent successfully!');
        })
        .catch(error => {
            console.error('Error:', error);
        });
});


