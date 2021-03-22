# Codelorians' Covid-19 Tracker Website

## Intro
This is the source code for my team's entry for the BEST Engineering Marathon,
written over the course of 24h in November 2020.

It is a website designed to aggregate real-time information about the Covid-19
pandemic.

Screenshots are provided in the screenshots/ folder.

## Features
Coronavirus heat-map using the Google Data Visualization API, up-to-date data.

News and Tweets aggregator that filters the latest Covid-related news and
tweets from around the internet.

Vaccine development progress and details provided by a public API.

Detailed per-country statistics, sortable by different criteria. Each country
has dynamically-generated graphs and some additional information.

## Technologies and Implementation
This Web app was written in Java using the Java Spring MVC framework.
Naturally, the code follows the Model-View-Controller design pattern to
service incoming HTTP requests.

The website is designed in HTML/CSS, along with some embedded JavaScript code.

Data is updated by performing HTTP requests to publicly available REST APIs
that track various Covid data (cases, vaccine development), along with filtered
results from a news api. Moreover, the section dedicated to tweets filters
live tweets based on language and content (using the public Twitter API) and
shows the latest relevant tweets.

Cases data are formatted into tables using the Google Data Visualization API and
some embedded Javascript.
