clang-uml
[ -d "puml" ] && echo "Directory puml exists." || mkdir puml
cd puml/
[ -e MultiRelay_class_diagram.png ] && rm MultiRelay_class_diagram.png
plantuml MultiRelay_class_diagram.puml
cd ..