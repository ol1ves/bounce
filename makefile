# ----------------------------
# Makefile Options
# ----------------------------

NAME = bounce
ICON = icon.png
DESCRIPTION = "dvd bouncer"
COMPRESSED = YES
COMPRESSED_MODE = zx7
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
