' GuessNum.bas

Font 5

num=Int(10*Rnd())
count = 1
guessAgain:
  Print "Guess number (0-9)":Input guess
  If guess = num Then GoTo gotIt
  If guess > num Then Print "You guessed too high"
  If guess < num Then Print "You guessed too low"
  count = count + 1
  GoTo guessAgain

gotIt:
Print "You got it in";count;" guesses"                                              