import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        file_p = open(positives)
        self.positives_list = []
        for line in file_p:
            new_line  = line.strip()
            if ((new_line == '') or (line.startswith(';') == True)):
                continue
            self.positives_list.append(new_line)
        file_p.close

        file_n = open(negatives)
        self.negatives_list = []
        for line in file_n:
            new_line  = line.strip()
            if ((new_line == '') or (line.startswith(';') == True)):
                continue
            self.negatives_list.append(new_line)
        file_n.close

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        for i in range(len(text)):
            token = text[i]
            token.lower
            for i in range(len(self.positives_list)):
                if self.positives_list[i] == token:
                    score += 1
            for i in range(len(self.negatives_list)):
                if self.negatives_list[i] == token:
                    score -= 1
        return score
