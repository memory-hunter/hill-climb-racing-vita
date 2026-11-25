from pathlib import Path
import unittest


class RenderLifecycleTests(unittest.TestCase):
    def test_render_loop_does_not_broadcast_foreground(self):
        source = (Path(__file__).resolve().parents[1] / "source/main.c").read_text()
        loop = source.split("while (1)", 1)[1].split("audio_destroy", 1)[0]
        native_init = "Game_nativeInit(&jni, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);"
        self.assertEqual(source.count(native_init), 1)
        self.assertNotIn(native_init, loop)
        self.assertNotIn("Game_nativeResize", source)
        self.assertIn("Game_nativeRender(&jni)", loop)
        self.assertIn("gl_swap()", loop)


if __name__ == "__main__":
    unittest.main()
