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
    Currently I'm working to design software mechanisms that 
    take advantage of the virtualization capabilities offered by the underlying
    hardware. In particular, my research addresses the use of virtualization to
    increase the security of a system.
</p>

<h2 id="publications">Publications</h2>

<p>
    Here you can find the list of article published on international conference
    and workshop proceedings. Here below you can find a (per-year) summary.
</p>

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
                    <b><a href="{{ pub.paper_link }}" target="_blank">{{ pub.title }}</a></b><br>
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
    will find both ones.
</p>

<ul class="tree-view">
    <li>List of theses</li>
    {% assign year = "2019" %}
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