---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults
section: publications
layout: page
style: page
permalink: /publications/
title: Publications
---

{% assign pubs = site.publications | sort: 'date' | reverse %}

<h2 id="interest">Research topics</h2>

<p>
    My research addresses the use of virtualization and state-of-the-art features offered by hardware vendors to increase the safety and security of a system. Therefore my main interests include safe and secure cyber-physical systems, operating systems and firmware, hypervisors, and edge computing.
</p>

<h2 id="publications">Publications</h2>

<p>
    Here below you can find a (per-year) summary of articles published on international conference/workshop proceedings
    and on international journals.
    For each article, you can access the preprint version (gears icon) or the published one (padlock icon).
</p>

<style>
    span > a::after {
        background-size: 15px 15px;
        display: inline-block;
        width: 15px;
        height: 15px;
        content:"";
        transform: translateY(3px);
    }
    span > a.preprint::after {
        background-image: url(/assets/img/gears-icon.png);
    }
    span > a.printed::after {
        background-image: url(/assets/img/padlock-icon.png);
    }
</style>

<ul class="tree-view">
    <li>List of publications</li>
    {% assign year =  "now" | date: "%Y" %}
    <li>
        <details open="">
            <summary>{{ year }}</summary>
            <ul>
            {% for pub in pubs %}
                {% if pub.publication_type == "paper" %}
                {% assign pub_year = pub.date | date: "%Y" %}
                {% if year != pub_year %}
                    {% assign year = pub_year %}
            </ul>
        </details>
    </li>
    <li>
        <details open="">
            <summary>{{ year }}</summary>
            <ul>
                {% endif %}
                <li>
                    {{ pub.authors }}<br>
                    <span>
                        <b>{{ pub.title }}</b>
                        {% if pub.paper_link %}
                        [<a class="preprint" href="{{ pub.paper_link }}" target="_blank"></a>]
                        {% endif %}
                        {% if pub.doi %}
                        [<a class="printed" href="{{ pub.doi }}" target="_blank"></a>]
                        {% endif %}
                    </span>
                    <br>
                    {{ pub.publication_name }}
                </li>
                {% endif %}
            {% endfor %}
            </ul>
        </details>
    </li>
</ul>

<h2 id="theses">Theses</h2>

<p>
    Both my Bachelor's degree and my Master's degree terminated with the 
    dissertation of a thesis. My Master thesis was covered by a three-year-NDA 
    agreement due to the involvement of an industrial company. Eventually, you 
    will find it.
</p>

<ul class="tree-view">
    <li>List of theses</li>
    {% assign year = "2023" %}
    <li>
        <details open="">
            <summary>{{ year }}</summary>
            <ul>
            {% for pub in pubs %}
                {% if pub.publication_type == "thesis" %}
                {% assign pub_year = pub.date | date: "%Y" %}
                {% if year != pub_year %}
                    {% assign year = pub_year %}
            </ul>
        </details>
    </li>
    <li>
        <details open="">
            <summary>{{ year }}</summary>
            <ul>
                {% endif %}
                <li>
                    {{ pub.authors }}<br>
                    <b><a href="{{ pub.paper_link }}" target="_blank">{{ pub.title }}</a></b><br>
                    {{ pub.publication_name }}
                </li>
                {% endif %}
            {% endfor %}
            </ul>
        </details>
    </li>
</ul>