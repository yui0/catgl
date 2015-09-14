#!/bin/sh
#git checkout -b gh-pages (初回のみ)

git checkout gh-pages
git rebase master
git push origin gh-pages

git checkout master

