from os.path import join, isfile
from os import mkdir

Import("env")
LIBDEPS_DIR = env['PROJECT_LIBDEPS_DIR']
patchflag_path = join(LIBDEPS_DIR, ".patching-done")

files_to_patch = dict()
files_to_patch[join(LIBDEPS_DIR, "pico", "PicoDVI - Adafruit Fork", "src", "libdvi", "dvi_config_defs.h")] = join("patches", "picodvi_adafruit_remove_config_h.patch")
files_to_patch[join(LIBDEPS_DIR, "pico", "PicoDVI - Adafruit Fork", "software", "include", "common_dvi_pin_configs.h")] = join("patches", "picodvi_adafruit_add_neo6502_gpio.patch")

# patch file only if we didn't do it before
if not isfile(patchflag_path):
    for original_file, patch_file in files_to_patch.items():                                                       
        assert isfile(original_file) and isfile(patch_file)
        env.Execute("patch '%s' '%s'" % (original_file, patch_file))

    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))