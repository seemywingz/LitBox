import React, { useEffect } from 'react';
import BrightnessSlider from './BrightnessSlider';
import FramerateSlider from './FramerateSlider';

function GameOfLifeSettings({ config, updateConfig }) {

    useEffect(() => {
        const newConfig = { ...config, visualization: 'gameOfLife' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);


    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            <FramerateSlider config={config} updateConfig={updateConfig} />
            {/* Add other components or controls */}
        </div>
    );
}

export default GameOfLifeSettings;