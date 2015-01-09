import sys
import scrabble_cheat
from scrabble_base_ai import scrabble_base_ai

class scrabble_suboptimal_ai(scrabble_base_ai):
    
    def generate_move_internal(self):
        candidates = scrabble_cheat.generate_word_candidates(self.board, self.config, self.hand)
        if candidates == []:
            return None
        if len(candidates) == 1:
            return candidates[0]
        return candidates[1]
