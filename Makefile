help:
	@echo "Usage:"
	@echo "    make install   | to install the GravitySim module"
	@echo "    make uninstall | to uninstall the GravitySim module"
	@echo "    make clean     | to remove build artifacts"

install:
	python -m pip install .

clean:
	python setup.py clean --all
	rm -rf build dist *.egg-info

uninstall:
	python -m pip uninstall -y GravitySim

reinstall: uninstall clean install

.PHONY: help install clean uninstall reinstall
