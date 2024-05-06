import React, { useState, useEffect } from 'react';
import WiFiSettings from './WiFiSettings';
import APSettings from './APSettings';
import FileSettings from './FileSettings';
import BarsSettings from './BarsSettings';
import GameOfLifeSettings from './GameOfLifeSettings';
import MatrixSettings from './MatrixSettings';
import WaveformSetting from './WaveformSettings';
import BirdsSetting from './BirdsSettings';
import CirclesSetting from './CirclesSetting';
import StarPulseSetting from './StarPulseSettings';
import TemperatureSetting from './TemperatureSettings';
import MotionSettings from './MotionSettings';

var defaultConfig = {
  "mode": "client",
  "mdns": "litbox",
  "client": {
    "ssid": "SSID",
    "password": "PASSWORD",
  },
  "ap": {
    "ssid": "LitBox-AP",
    "password": "abcd1234"
  },
  "brightness": 18,
  "sensitivity": 9,
  "visualization": "bars",
  "frameRate": 30,
  "text": "LitBox",
  "colorPallet": [31, 2047, 63514, 65535],
  "pixelColor": 65535,
  "pixelBgColor": 0
};

function App() {
  var [config, setConfig] = useState(null);
  const [selectedSetting, setSelectedSetting] = useState('bars');  // Default to 'about'

  useEffect(() => {
    fetch('/config')
      .then(response => response.json())
      .then(data => setConfig(data))
      .catch(error => console.error('Error loading configuration:', error));
  }, []);

  const saveConfig = (newConfig) => {
    fetch('/config', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(newConfig),
    }).then(response => {
      if (!response.ok) throw new Error('Failed to save configuration');
      return response.json();
    }).then(data => {
      setConfig(newConfig);
      console.log('Configuration updated:', data);
      alert('Configuration updated');
    }).catch(error => console.error('Error updating configuration:', error));
  };

  const updateConfig = (newConfig) => {
    fetch('/updateConfig', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(newConfig),
    }).then(response => {
      if (!response.ok) throw new Error('Failed to update configuration');
      return response.json();
    }).then(data => {
      setConfig(newConfig);
      console.log('Configuration updated:', data);
      // alert('Configuration updated');
    }).catch(error => console.error('Error updating configuration:', error));
  };

  // if (!config) return <div>Loading...</div>;

  if (!config) {
    config = defaultConfig;
  }

  var underConstruction = <div>🛠️ Under Construction 🛠️</div>;

  const renderSetting = () => {
    switch (selectedSetting) {
      case 'bars':
        return <BarsSettings config={config} updateConfig={updateConfig} />;
      case 'birds':
        return <BirdsSetting config={config} updateConfig={updateConfig} />;
      case 'circles':
        return <CirclesSetting config={config} updateConfig={updateConfig} />;
      case 'gameOfLife':
        return <GameOfLifeSettings config={config} updateConfig={updateConfig} />;
      case 'matrix':
        return <MatrixSettings config={config} updateConfig={updateConfig} />;
      case 'motion':
        return <MotionSettings config={config} updateConfig={updateConfig} />;
      case 'starPulse':
        return <StarPulseSetting config={config} updateConfig={updateConfig} />;
      case 'text':
        return underConstruction
      case 'temperature':
        return <TemperatureSetting config={config} updateConfig={updateConfig} />;
      case 'waveform':
        return <WaveformSetting config={config} updateConfig={updateConfig} />;
      case 'wifi':
        return <WiFiSettings config={config} saveConfig={saveConfig} />;
      case 'ap':
        return <APSettings config={config} saveConfig={saveConfig} />;
      case 'files':
        return <FileSettings />;
      case 'about':
        return <div class="setting" id="about-settings">

          <div class="setting">
            <label>Version</label>
            <label id="version">{config.version}</label>
          </div>

          <div class="setting">
            <label>Designed and Developed by:</label>
            <a href="https://github.com/seemywingz/LitBox"
              target="_blank" id="SeeMyWingZ" rel="noreferrer">SeeMyWingZ</a>
          </div>

        </div>;
      default:
        return underConstruction
    }
  };

  return (
    <div className="container">
      <label className="header">Lit Box</label>
      <div className="setting">
        <select
          id="settingSelect"
          className="clickable"
          onChange={(e) => setSelectedSetting(e.target.value)}
        >
          <option value="bars">Bars</option>
          <option value="birds">Birds</option>
          <option value="circles">Circles</option>
          <option value="gameOfLife">Game of Life</option>
          <option value="matrix">Matrix</option>
          <option value="motion">Motion</option>
          <option value="starPulse">Star Pulse</option>
          <option value="text">Text</option>
          <option value="temperature">Temperature</option>
          <option value="waveform">Waveform</option>
          <option value="wifi">WiFi</option>
          <option value="ap">AP</option>
          <option value="files">Files</option>
          <option value="about">About</option>
        </select>
      </div>
      {renderSetting()}
    </div>
  );
}

export default App;