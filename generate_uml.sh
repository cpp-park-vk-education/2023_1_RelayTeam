clang-uml
[ -d "puml" ] && echo "Directory puml exists." || mkdir puml
cd puml/
[ -e MultiRelay_class_diagram.png ] && rm MultiRelay_class_diagram.png
[ -e MultiRelay_class_diagram.png ] && rm MultiRelay_class_diagram.svg
plantuml MultiRelay_class_diagram.puml
plantuml MultiRelay_class_diagram.puml -tsvg
[ -e gstreamer_interface_class_diagram.png ] && rm gstreamer_interface_class_diagram.png
[ -e gstreamer_interface_class_diagram.svg ] && rm gstreamer_interface_class_diagram.svg
plantuml gstreamer_interface_class_diagram.puml
plantuml gstreamer_interface_class_diagram.puml -tsvg
[ -e search_devices_class_diagram.png ] && rm search_devices_class_diagram.png
[ -e search_devices_class_diagram.svg ] && rm search_devices_class_diagram.svg
plantuml search_devices_class_diagram.puml
plantuml search_devices_class_diagram.puml -tsvg
[ -e application_class_diagram.png ] && rm application_class_diagram.png
[ -e application_class_diagram.svg ] && rm application_class_diagram.svg
plantuml application_class_diagram.puml
plantuml application_class_diagram.puml -tsvg
cd ..
