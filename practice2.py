class Candidate:
    def __init__(self, name):
        self.name = name
        self.votes = 0

    def add_vote(self):
        self.votes += 1

class Voter:
    def __init__(self, name):
        self.name = name
        self.votes_cast = 0

    def vote(self, candidate):
        if self.votes_cast < 2:
            candidate.add_vote()
            self.votes_cast += 1

kaja = Candidate("Kaja")
urmas = Candidate("Urmas")
martin = Candidate("Martin")

jaanus = Voter("Jaanus")
urve = Voter("Urve")
ain = Voter("Ain")
maarja = Voter("Maarja")
siim = Voter("Siim")

jaanus.vote(kaja)
urve.vote(urmas)
ain.vote(martin)
maarja.vote(kaja)
siim.vote(kaja)
siim.vote(martin)
maarja.vote(urmas)
jaanus.vote(urmas)
ain.vote(kaja)

candidates = [kaja, urmas, martin]
for candidate in candidates:
    print(f"{candidate.name} has {candidate.votes} votes.")

winner = max(candidates, key=lambda candidate: candidate.votes)
print(f"The winner is {winner.name}.")
