package com.covid.codelorians.utils;

public class NumberUtil {
    public static String bigNumberFormat(int number) {
        String s = Integer.toString(number);
        String newS = "";
        int count = 0;
        for (int i = s.length() - 1; i >= 0; i--) {
            count++;
            newS = s.charAt(i) + newS;
            if (count % 3 == 0 && i != 0) {
                newS = "." + newS;
            }
        }
        return newS;
    }
}
