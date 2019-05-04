import os
import logging
import logging.handlers


BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

handler = logging.handlers.WatchedFileHandler(os.path.join(BASE_DIR, 'nano.log'))
formatter = logging.Formatter(logging.BASIC_FORMAT)
handler.setFormatter(formatter)
root = logging.getLogger()
root.setLevel("DEBUG")
root.addHandler(handler)
