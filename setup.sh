#!/bin/bash
# compile gresource and gschema

cd ../
echo compiling gresource
glib-compile-resources --target=resources.c --generate-source timetodo.gresource.xml

echo copying and exporting gschema
cp org.gtkmm.timetodo.gschema.xml /home/david/.local/share/glib-2.0/schemas
export GSETTINGS_SCHEMA_DIR=~/.local/share/glib-2.0/schemas
echo compiling gschema
glib-compile-schemas /home/david/.local/share/glib-2.0/schemas

cd build/