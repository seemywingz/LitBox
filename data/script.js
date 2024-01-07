function fetchSliderSettings() {
    fetch('/sensitivity', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            sensitivitySlider.value = parseInt(data, 10); // Update the slider's value
        })
        .catch(error => {
            console.error('Error fetching initial sensitivity:', error);
        });

    fetch('/brightness', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            brightnessSlider.value = parseInt(data, 10); // Update the slider's value
        })
        .catch(error => {
            console.error('Error fetching initial brightness:', error);
        });

    fetch('/frameRate', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            frameRateSlider.value = parseInt(data, 10); // Update the slider's value
        })
        .catch(error => {
            console.error('Error fetching initial framerate:', error);
        });

    fetch('/visualization', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            document.getElementById('visualizationSelect').value = data;
            visualizationSelect.dispatchEvent(new Event('change'));
        })
        .catch(error => {
            console.error('Error fetching initial visualization:', error);
        });

    fetch('/motion', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var settings = data.split('\n');
            settings.forEach(setting => {
                var parts = setting.split('=');
                var key = parts[0];
                var value = parts[1];
                switch (key) {
                    case 'motionNumObjects':
                        document.getElementById('motionNumObjects').value = value;
                        break;
                }
            });
        })


    fetch('/starPulse', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var settings = data.split('\n');
            settings.forEach(setting => {
                var parts = setting.split('=');
                var key = parts[0];
                var value = parts[1];
                switch (key) {
                    case 'starCount':
                        document.getElementById('starCount').value = value;
                        break;
                }
            });
        })
        .catch(error => {
            console.error('Error fetching bird settings:', error);
        });
}

function fetchBirdSettings() {
    fetch('/birds', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var settings = data.split('\n');
            settings.forEach(setting => {
                var parts = setting.split('=');
                var key = parts[0];
                var value = parts[1];
                switch (key) {
                    case 'birdCount':
                        document.getElementById('birdCount').value = value;
                        break;
                    case 'birdAlignment':
                        document.getElementById('birdAlignment').value = value;
                        break;
                    case 'birdCohesion':
                        document.getElementById('birdCohesion').value = value;
                        break;
                    case 'birdSeparation':
                        document.getElementById('birdSeparation').value = value;
                        break;
                    case 'birdVerticalBounds':
                        document.querySelector('#birdVerticalBounds').checked = (value === 1);
                        break;
                    case 'birdHorizontalBounds':
                        document.querySelector('#birdHorizontalBounds').checked = (value === 1);
                        break;
                }
            });
        })
        .catch(error => {
            console.error('Error fetching bird settings:', error);
        });
}

function fetchColorSettings() {
    fetch('/colors', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var settings = data.split('\n');
            settings.forEach(setting => {
                var parts = setting.split('=');
                var key = parts[0];
                var value = parts[1];
                switch (key) {
                    case 'color1':
                        document.getElementById('color1').value = value;
                        break;
                    case 'color2':
                        document.getElementById('color2').value = value;
                        break;
                    case 'color3':
                        document.getElementById('color3').value = value;
                        break;
                    case 'color4':
                        document.getElementById('color4').value = value;
                        break;
                    case 'pixelColor':
                        document.getElementById('pixelColor').value = value;
                        break;
                    case 'pixelBgColor':
                        document.getElementById('pixelBgColor').value = value;
                        break;
                }
            });
        })
        .catch(error => {
            console.error('Error fetching color settings:', error);
        });
}

function fetchTemperatureSettings() {
    fetch('/temperature', { method: 'GET' })
        .then(response => response.text())
        .then(data => {
            var settings = data.split('\n');
            settings.forEach(setting => {
                var parts = setting.split('=');
                var key = parts[0];
                var value = parts[1];
                switch (key) {
                    case 'temperatureUnit':
                        document.getElementById('temperatureSelect').value = value;
                        break;
                    case 'temperature':
                        var unit = document.getElementById('temperatureSelect').value;
                        document.getElementById('temperature').textContent = "Temp: " + value + "°" + unit;
                        break;
                }
            });
        })
        .catch(error => {
            console.error('Error fetching temperature settings:', error);
        });
}

window.onload = function () {
    fetchTemperatureSettings();
    fetchSliderSettings();
    fetchColorSettings();
    fetchBirdSettings();
}

document.querySelectorAll('.toggle-button').forEach(function (toggle) {
    var contentId = toggle.id;
    // Load the saved state
    toggle.checked = localStorage.getItem(contentId) === 'true';
    // Save state on change
    toggle.addEventListener('click', function () {
        localStorage.setItem(contentId, toggle.checked);
    });
});

var sensitivitySlider = document.getElementById('sensitivityValue');
sensitivitySlider.addEventListener('change', function () {
    var formData = new FormData();
    formData.append('sensitivity', sensitivitySlider.value);
    fetch('/sensitivity', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting sensitivity:', error);
        });
});

var brightnessSlider = document.getElementById('brightnessValue');
brightnessSlider.addEventListener('change', function () {
    var formData = new FormData();
    formData.append('brightness', brightnessSlider.value);
    fetch('/brightness', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting brightness:', error);
        });
});

var frameRateSlider = document.getElementById('frameRateValue');
frameRateSlider.addEventListener('change', function () {
    var formData = new FormData();
    formData.append('frameRate', frameRateSlider.value);
    fetch('/frameRate', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting frame rate:', error);
        });
});

var temperatureSelect = document.getElementById('temperatureSelect');
temperatureSelect.addEventListener('change', function () {
    var temperatureUnit = temperatureSelect.value;
    var formData = new FormData();
    formData.append('temperatureUnit', temperatureUnit);
    fetch('/temperature', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting temperature:', error);
        });
    fetchTemperatureSettings();
});

var visualizationSelect = document.getElementById('visualizationSelect');
visualizationSelect.addEventListener('change', function () {
    var visualization = visualizationSelect.value;

    var brightnessSettings = document.getElementById('brightness-settings');
    brightnessSettings.style.display = 'block';

    var birdSettings = document.getElementById('bird-settings');
    birdSettings.style.display = 'none';

    var textSettings = document.getElementById('text-settings');
    textSettings.style.display = 'none';

    var audioSettings = document.getElementById('audio-settings');
    audioSettings.style.display = 'none';

    var animationSettings = document.getElementById('animation-settings');
    animationSettings.style.display = 'none';

    var colorSettings = document.getElementById('color-settings');
    colorSettings.style.display = 'none';

    var colorsSettings = document.getElementById('colors-settings');
    colorsSettings.style.display = 'none';

    var starPulseSettings = document.getElementById('starPulse-settings');
    starPulseSettings.style.display = 'none';

    var temperatureSettings = document.getElementById('temperature-settings');
    temperatureSettings.style.display = 'none';

    var motionSettings = document.getElementById('motion-settings');
    motionSettings.style.display = 'none';

    var wifiSettings = document.getElementById('wifi-settings');
    wifiSettings.style.display = 'none';

    var aboutSettings = document.getElementById('about-settings');
    aboutSettings.style.display = 'none';

    switch (visualization) {
        case 'bars':
            audioSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'birds':
            birdSettings.style.display = 'block';
            animationSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'circles':
            audioSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'gameOfLife':
            animationSettings.style.display = 'block';
            colorSettings.style.display = 'block';
            break;
        case 'matrix':
            animationSettings.style.display = 'block';
            break;
        case 'motion':
            motionSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'starPulse':
            starPulseSettings.style.display = 'block';
            audioSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'text':
            textSettings.style.display = 'block';
            colorSettings.style.display = 'block';
            break;
        case 'temperature':
            temperatureSettings.style.display = 'block';
            animationSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'waveform':
            audioSettings.style.display = 'block';
            colorsSettings.style.display = 'block';
            break;
        case 'wifi':
            wifiSettings.style.display = 'block';
            brightnessSettings.style.display = 'none';
            break;
        case 'about':
            aboutSettings.style.display = 'block';
            brightnessSettings.style.display = 'none';
            break;
    }

    if (visualization !== 'wifi' && visualization !== 'text' && visualization !== 'about') {
        setVisualization(visualization);
    }
});


function setVisualization(visualization) {
    fetch('/visualization', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'visualization=' + encodeURIComponent(visualization)
    })
        .then(response => response.text())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error setting visualization mode:', error);
        });
}

document.getElementById('sendText').addEventListener('click', function () {
    var text = document.getElementById('customText').value;
    var textColor = document.getElementById('pixelColor').value;
    var textSpeed = document.getElementById('textSpeed').value;
    var textAnimation = document.getElementById('textAnimation').value;
    console.log('Sending text:', text, textColor, textSpeed);

    fetch('/text', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'text=' + encodeURIComponent(text)
            + '&textColor=' + encodeURIComponent(textColor)
            + '&textSpeed=' + encodeURIComponent(textSpeed)
            + '&textAnimation=' + encodeURIComponent(textAnimation)
    })
        .then(response => response.text())
        .then(data => console.log('Text and speed updated:', data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('updateStarPulse').addEventListener('click', function () {
    var starCount = document.getElementById('starCount').value;

    fetch('/starPulse', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'starCount=' + encodeURIComponent(starCount)
    })
        .then(response => response.text())
        .then(data => console.log('Star pulse updated:', data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('updateMotion').addEventListener('click', function () {
    var motionNumObjects = document.getElementById('motionNumObjects').value;

    fetch('/motion', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'motionNumObjects=' + encodeURIComponent(motionNumObjects)
    })
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('updateBirds').addEventListener('click', function () {
    var birdCount = document.getElementById('birdCount').value;
    var birdAlignment = document.getElementById('birdAlignment').value;
    var birdCohesion = document.getElementById('birdCohesion').value;
    var birdSeparation = document.getElementById('birdSeparation').value;
    var birdVerticalBounds = document.getElementById('birdVerticalBounds').checked;
    var birdHorizontalBounds = document.getElementById('birdHorizontalBounds').checked;

    fetch('/birds', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'birdCount=' + birdCount
            + '&birdAlignment=' + birdAlignment
            + '&birdCohesion=' + birdCohesion
            + '&separation=' + birdSeparation
            + '&birdVerticalBounds=' + birdVerticalBounds
            + '&birdHorizontalBounds=' + birdHorizontalBounds
    })
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
});

document.getElementById('wifiSetup').addEventListener('click', function () {
    window.location.href = '/setup';
});

document.getElementById('wifiErase').addEventListener('click', function () {
    window.location.href = '/erase';
});

// for each color picker input field add an event listener to update the color
document.querySelectorAll('.color-picker').forEach(function (colorPicker) {
    colorPicker.addEventListener('change', function () {
        var color1 = document.getElementById('color1').value;
        var color2 = document.getElementById('color2').value;
        var color3 = document.getElementById('color3').value;
        var color4 = document.getElementById('color4').value;
        var pixelColor = document.getElementById('pixelColor').value;
        var pixelBgColor = document.getElementById('pixelBgColor').value;
        updateColor(color1, color2, color3, color4, pixelColor, pixelBgColor);
    });
});
function updateColor(color1, color2, color3, color4, pixelColor, pixelBgColor) {

    fetch('/colors', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
        },
        body: 'color1=' + color1
            + '&color2=' + color2
            + '&color3=' + color3
            + '&color4=' + color4
            + '&pixelColor=' + pixelColor
            + '&pixelBgColor=' + pixelBgColor
    })
        .then(response => response.text())
        .then(data => console.log(data))
        .catch(error => console.error('Error:', error));
}