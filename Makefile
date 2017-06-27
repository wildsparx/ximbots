# Copyright (C) 2017 Asher Blum

INCS := $(shell ls *.inc)

ALL: 1.html

levels.auto:
	python2 mk_levels.py > $@

demos.auto:
	python2 compile_demos.py a.commands.log > $@

sound/sounds.auto:
	cd sound && make

1.html: levels.auto demos.auto pre.html sound/sounds.auto $(INCS)
	python2 proc.py pre.html $@

clean:
	rm -f *.auto 1.html

foo:
	echo $(INCS)
