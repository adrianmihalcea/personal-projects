package com.covid.codelorians.services;

import com.covid.codelorians.models.CovidArticle;
import com.covid.codelorians.models.VaccineStats;
import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVRecord;
import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.io.StringReader;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.ArrayList;
import java.util.List;

import static com.covid.codelorians.constants.Constants.NEWS_URL;

// Service for collecting Covid News from API
@Service
public class CovidNewsDataService {
    public List<CovidArticle> allArticles = new ArrayList<>();

    @PostConstruct
    @Scheduled(cron = "* 1 * * * *")
    public void fetchData() throws IOException, InterruptedException {
        List<CovidArticle> newArticles = new ArrayList<>();
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().uri(URI.create(NEWS_URL)).build();
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());

        JSONObject obj = new JSONObject(response.body());
        JSONArray arr = obj.getJSONArray("articles");

        for (int i = 0; i < arr.length(); i++) {
            newArticles.add(new CovidArticle(arr.getJSONObject(i), i + 1));
        }

        this.allArticles = newArticles;
    }
}
