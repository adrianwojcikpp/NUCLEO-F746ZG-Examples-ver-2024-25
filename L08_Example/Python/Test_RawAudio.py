from pydub import AudioSegment
from pydub.playback import play

# Load raw audio file
audio = AudioSegment.from_file("audio.raw", format="raw", frame_rate=8000, sample_width=1, channels=1)

# Play audio
play(audio)


