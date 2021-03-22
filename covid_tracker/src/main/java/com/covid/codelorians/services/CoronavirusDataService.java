package com.covid.codelorians.services;

import com.covid.codelorians.models.CountryStats;
import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVRecord;
import org.springframework.data.util.Pair;
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
import java.util.function.Function;

import static com.covid.codelorians.constants.Constants.CASES_URL;
import static com.covid.codelorians.constants.Constants.DEATHS_URL;

// Service handling historical and latest data for infections and deaths
@Service
public class CoronavirusDataService {
    public List<CountryStats> allStats = new ArrayList<>();
    public List<Pair<String, Integer>> mapData = new ArrayList<>();

    @PostConstruct
    @Scheduled(cron = "* 1 * * * *")
    public void fetchData() throws IOException, InterruptedException {

        // Cases
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().uri(URI.create(CASES_URL)).build();
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        StringReader reader = new StringReader(response.body());
        Iterable<CSVRecord> records = CSVFormat.DEFAULT.withFirstRecordAsHeader().parse(reader);

        ArrayList<String> states = new ArrayList<>();
        ArrayList<String> countries = new ArrayList<>();
        ArrayList<ArrayList<Integer>> cases = new ArrayList<>();

        for (CSVRecord record : records) {
            states.add(record.get("Province/State"));
            countries.add(record.get("Country/Region"));
            ArrayList<Integer> temp = new ArrayList<>();
            for (int i = 4; i < record.size(); ++i) {
                temp.add(Integer.parseInt(record.get(i)));
            }
            cases.add(temp);
        }

        // Deaths
        request = HttpRequest.newBuilder().uri(URI.create(DEATHS_URL)).build();
        response = client.send(request, HttpResponse.BodyHandlers.ofString());
        reader = new StringReader(response.body());
        records = CSVFormat.DEFAULT.withFirstRecordAsHeader().parse(reader);

        ArrayList<ArrayList<Integer>> deaths = new ArrayList<>();

        for (CSVRecord record : records) {
            ArrayList<Integer> temp = new ArrayList<>();
            for (int i = 4; i < record.size(); ++i) {
                temp.add(Integer.parseInt(record.get(i)));
            }
            deaths.add(temp);
        }

        for (int i = 0; i < deaths.size(); ++i) {
            allStats.add(new CountryStats(states.get(i), countries.get(i), i, cases.get(i), deaths.get(i)));
        }

        // Process data for map
        mapData = setMap(CountryStats::getTotalCases);
    }

    private List<Pair<String, Integer>> setMap(Function<CountryStats, Integer> f) {
        List<Pair<String, Integer>> mapData = new ArrayList<>();
        mapData.add(Pair.of(allStats.get(0).getCountry(), f.apply(allStats.get(0))));
        Pair<String, Integer> lastEntry = Pair.of("", 0);
        for (int i = 1; i < allStats.size(); ++i) {
            CountryStats entry = allStats.get(i);
            if (entry.getCountry().equals(lastEntry.getFirst())) {
                int temp = lastEntry.getSecond() + entry.getTotalCases();
                mapData.set(mapData.size() - 1, Pair.of(entry.getCountry(), temp));
                lastEntry = Pair.of(lastEntry.getFirst(), temp);
            } else {
                lastEntry = Pair.of(entry.getCountry(), entry.getTotalCases());
                mapData.add(lastEntry);
            }
        }
        return mapData;
    }
}
