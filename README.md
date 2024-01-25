# espeak-python

TODO: Read and obey espeak license!

TODO: Static link for portability (better than the other implementation)
TODO: emacs dired configurations for building and lsp stuff

# Supports
* PLAYBACK mode, not RETRIEVAL. If you just want to play the sound instead of handling the audio data and synthesizing yourself, it works just fine.



# Commands
(https://blog.ganssle.io/articles/2021/10/setup-py-deprecated.html#summary)
setup.py command	New command
setup.py sdist
setup.py bdist_wheel
python -m build (with build)
setup.py test	pytest (usually via tox or nox)
setup.py install	pip install
setup.py develop	pip install -e
setup.py upload	twine upload (with twine)
setup.py check	twine check (this doesn't do all the same checks but it's a start)
Custom commands	tox and nox environments.
