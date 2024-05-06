import React, { useEffect } from 'react';
import BrightnessSlider from './BrightnessSlider';

function BarsSettings({ config, updateConfig }) {
    // Memoize the updateConfig function

    // eslint-disable-next-line
    useEffect(() => {
        const newConfig = { ...config, visualization: 'bars' };
        updateConfig(newConfig);
        return () => {
            // perform clean-up tasks here if needed
        };
    }, []);


    return (
        <div className="setting">
            <BrightnessSlider config={config} updateConfig={updateConfig} />
            {/* Add other components or controls */}
        </div>
    );
}

export default BarsSettings;
