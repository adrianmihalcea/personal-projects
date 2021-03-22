package com.covid.codelorians.models;

import com.covid.codelorians.utils.NumberUtil;

import java.util.ArrayList;

public class CountryStats implements Comparable<CountryStats>{
    private static int latestGrandTotal = 0;
    private String state;
    private String country;
    private int id, rank;
    // cases
    private ArrayList<Integer> extraCases, casesUntil;
    private double incProportion;
    // deaths
    private ArrayList<Integer> extraDeaths, deathsUntil;

    public CountryStats(String state, String country, int id, ArrayList<Integer> casesUntil,
                        ArrayList<Integer> deathsUntil) {
        this.state = state;
        this.country = country;
        this.id = id;
        this.casesUntil = casesUntil;
        this.deathsUntil = deathsUntil;
        setExtraCasesDeaths();
        setProportion();
    }

    public static int getLatestGrandTotal() {
        return latestGrandTotal;
    }

    // Computes Infection Rate for last 2 weeks
    private void setProportion() {
        double average = 0;
        int count = 0;
        for (int i = extraCases.size() - 1; i >= extraCases.size() - 14; i--) {
            if (extraCases.get(i - 1) == 0 || extraCases.get(i) == 0) {
                continue;
            }
            average += ((double) extraCases.get(i) / extraCases.get(i - 1));
            count++;
        }
        incProportion = average / count;
    }

    // Readable format for infection rate
    public String getProportion() {
        return String.format("%.3f", incProportion);
    }

    // Computes daily difference in infections and deaths
    public void setExtraCasesDeaths() {
        extraDeaths = new ArrayList<>();
        extraCases = new ArrayList<>();
        extraDeaths.add(0);
        extraCases.add(0);
        for (int i = 1; i < casesUntil.size(); ++i) {
            extraDeaths.add(deathsUntil.get(i) - deathsUntil.get(i - 1));
            extraCases.add(casesUntil.get(i) - casesUntil.get(i - 1));
        }
        latestGrandTotal += extraCases.get(casesUntil.size() - 1);
    }

    // Readable format for big numbers
    public String showNumber(int number) {
        return NumberUtil.bigNumberFormat(number);
    }

    public int getLastDeathIncrease() {
        int size = deathsUntil.size();
        return deathsUntil.get(size - 1) - deathsUntil.get(size - 2);
    }

    public String getState() {
        return state;
    }

    public String getCountry() {
        return country;
    }

    public int getId() {
        return id;
    }

    public int getRank() {
        return rank;
    }

    public ArrayList<Integer> getExtraCases() {
        return extraCases;
    }

    public ArrayList<Integer> getCasesUntil() {
        return casesUntil;
    }

    public ArrayList<Integer> getExtraDeaths() {
        return extraDeaths;
    }

    public ArrayList<Integer> getDeathsUntil() {
        return deathsUntil;
    }

    public int getLastCaseIncrease() {
        int size = casesUntil.size();
        return casesUntil.get(size - 1) - casesUntil.get(size - 2);
    }

    public int getTotalDeaths() {
        return deathsUntil.get(deathsUntil.size() - 1);
    }

    public int getTotalCases() {
        return casesUntil.get(casesUntil.size() - 1);
    }

    @Override
    public String toString() {
        return "LocationStats{" +
                "state='" + state + '\'' +
                ", country='" + country + '\'' +
                ", latestTotalCases=" + getTotalCases() +
                '}';
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    @Override
    public int compareTo(CountryStats obj) {
        return Integer.compare(getLastCaseIncrease(), obj.getLastCaseIncrease());
    }

    public static int compareAlpha(CountryStats jc1, CountryStats jc2) {
        return (int) (jc1.getCountry().compareTo(jc2.getCountry()));
    }
}