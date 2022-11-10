# https://stackoverflow.com/a/36683917
define GetFromConfig
$(shell node -p "require('./config.json').$(1)")
endef

GODOT_CPP_PATH := $(call GetFromConfig,godotcpp.path)
SCONS_LAB = python3 /usr/bin/scons
BINDINGS_FLAGS = -C "${GODOT_CPP_PATH}" -j4 bits=64

bindings-lab:
	${SCONS_LAB} ${BINDINGS_FLAGS}

bindings:
	scons ${BINDINGS_FLAGS}
	
plugin-lab:
	${SCONS_LAB} ./src p=x11

plugin:
	scons ./src p=$(p)
