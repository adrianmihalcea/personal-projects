package com.covid.codelorians.models;

import org.json.JSONArray;
import org.json.JSONObject;

public class Tweet {
    private String author, text, profilePicUrl;

    public Tweet(JSONObject object) {
        text = object.getJSONObject("data").get("text").toString();
        author = object.getJSONObject("includes").getJSONArray("users").getJSONObject(0).getString("name");
        profilePicUrl = object.getJSONObject("includes").getJSONArray("users").getJSONObject(0).getString("profile_image_url");
    }

    public String getAuthor() {
        return author;
    }

    public String getText() {
        return text;
    }

    public String getProfilePicUrl() {
        return profilePicUrl;
    }

    public String getPic() {
        return profilePicUrl;
    }
}
