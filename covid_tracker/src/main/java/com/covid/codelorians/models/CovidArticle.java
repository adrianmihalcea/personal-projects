package com.covid.codelorians.models;

import org.json.JSONObject;

// Class used for storing info about individual news articles
public class CovidArticle {
    private String sourceName, author, articleTitle, description, url, imageUrl, date, content;
    private int id;

    public CovidArticle(JSONObject object, int id) {
        sourceName = object.getJSONObject("source").get("name").toString();
        author = object.get("author").toString();
        articleTitle = object.get("title").toString();
        description = object.get("description").toString();
        url = object.get("url").toString();
        imageUrl = object.get("urlToImage").toString();
        date = object.get("publishedAt").toString();
        content = object.get("content").toString();
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public String getSourceName() {
        return sourceName;
    }

    public String getAuthor() {
        return author;
    }

    public String getArticleTitle() {
        return articleTitle;
    }

    public String getDescription() {
        return description;
    }

    public String getUrl() {
        return url;
    }

    public String getImageUrl() {
        return imageUrl;
    }

    public String getDate() {
        return date;
    }

    public String getContent() {
        return content;
    }
}

