# This is probably redundant
if [ ! -d "bin" ]; then
    mkdir bin
fi

# Export aseprite file if found
if ! command -v aseprite > /dev/null
then
    printf "Aseprite not found and needs to be added to the PATH.\n"
    exit 1
fi
aseprite -b build/aseprite/atlas.aseprite --save-as build/tmp/atlas_tmp.bmp

# Split colors into primary and secondary
# Two steps for primary to turn remaining blue -> white
build/bmpe build/tmp/atlas_tmp.bmp build/tmp/primary_tmp.bmp -r 0xffffffff 0xff000000
build/bmpe build/tmp/primary_tmp.bmp bin/atlas_primary.bmp -r 0xffff0000 0xffffffff
# Secondary color already white in atlas_tmp
build/bmpe build/tmp/atlas_tmp.bmp bin/atlas_secondary.bmp -r 0xffff0000 0xff000000
# Cleanup temp files
rm build/tmp/atlas_tmp.bmp
rm build/tmp/primary_tmp.bmp
