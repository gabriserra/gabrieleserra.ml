---
layout: page
title: Projects
style: blog
permalink: /projects/
---

<div class="archive">
  {% assign year = "now" | date: "%Y" %}
  <h2 class="archive-year">{{ year }}</h2>
  {% for post in site.posts %}
    {% assign post_year = post.date | date: "%Y" %}
    {% if year != post_year %}
      {% assign year = post_year %}
      <h2 class="archive-year">{{ year }}</h2>
    {% endif %}
    <div class="archive-item">
      <span class="post-date archive-date">{{ post.date | date: "%b %d, %y" }}</span>
      <a href="{{ post.url }}" class="archive-title">
        {{ post.title }}
      </a>
      {% for category in post.categories %}
        <span class="post-category {{ category }}"></span>
      {% endfor %}
    </div>
  {% endfor %}
</div>