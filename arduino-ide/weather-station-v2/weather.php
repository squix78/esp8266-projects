<?php
        include('lib/forecast.io.php');
        $apiKeyParam = $_GET["apiKey"];
        $latParam = $_GET["lat"];
        $lonParam = $_GET["lon"];
        $unitsParam = $_GET["units"];
        $langParam = $_GET["lang"];

        $units = 'auto';  // Can be set to 'us', 'si', 'ca', 'uk' or 'auto' (see forecast.io API); default is auto
        $lang = 'en'; // Can be set to 'en', 'de', 'pl', 'es', 'fr', 'it', 'tet' or 'x-pig-latin' (see forecast.io API); default is 'en'

        if($unitsParam != "") {
                $units = $unitsParam;
        }
        if($langParam != "") {
                $lang = $langParam;
        }
        error_log(date(DATE_RFC822)." -- api=".$apiKeyParam.",lat=".$latParam.",lon=".$lonParam.",units=".$units.",lang=".$lang."\n", 3, '/home/squixor/weather.log');
        $forecast = new ForecastIO($apiKeyParam, $units, $lang);
        $condition = $forecast->getCurrentConditions($latParam, $lonParam);
        echo "CURRENT_TEMP=".round($condition->getTemperature())."\n";
        echo "CURRENT_HUMIDITY=".($condition->getHumidity()*100)."\n";
        echo "CURRENT_ICON=".($condition->getIcon())."\n";
        echo "CURRENT_SUMMARY=".$condition->getSummary()."\n";

        $conditions_week = $forecast->getForecastWeek($latParam, $lonParam);
        echo "MAX_TEMP_TODAY=".round($conditions_week[0]->getMaxTemperature()) . "\n";
        echo "MIN_TEMP_TODAY=".round($conditions_week[0]->getMinTemperature()) . "\n";
        echo "ICON_TODAY=".$conditions_week[0]->getIcon()."\n";
        echo "SUMMARY_TODAY=".$conditions_week[0]->getSummary()."\n";
        echo "MAX_TEMP_TOMORROW=" . round($conditions_week[1]->getMaxTemperature()) . "\n";
        echo "ICON_TOMORROW=".$conditions_week[1]->getIcon()."\n";
        echo "MIN_TEMP_TOMORROW=".round($conditions_week[1]->getMinTemperature()) . "\n";
        echo "SUMMARY_TODAY=".$conditions_week[1]->getSummary()."\n";
?>
