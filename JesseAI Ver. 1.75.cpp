//Jesse Artificial Intelligence or J-A-I - Version 1.75
//Copyright (C) 2018 Written by D.B. Taylor - GPL 3.0 or Later
//http://jesse.thejoyfulprogrammer.com
//davidtaylorjai@gmail.com
//

#include <iostream>
#include <cstring>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdexcept>

void SetPalette(UINT8 PaletteNum);
void RandomizeConsoleBackground();
void ClearConsoleToEndOfLine();
void ClearScreen();
void Color(UINT8 Foreground);
void Color(UINT8 Foreground, UINT8 Background);
void FunkyConsoleBackground();

std::string RemovePunctuation (std::string &a);

std::ofstream JesseLog;

HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
CONSOLE_SCREEN_BUFFER_INFOEX csbiEX;

typedef struct IR {
    std::string PreDefInput;
    std::string Response;
} InputAndResponses;


namespace Jesse
{
    COORD CursorCoords = {0, 0};
    COORD NewScreenBufferDimensions = {81, 6000}; // Console Buffer Width, Height
    SMALL_RECT NewsrWindow = {0, 0, 81, 40};      // New Console Window StartX, StartY, EndX, EndY
    COORD MaximumWindowSize = {81, 40};           // Console Window Maximum Width, Height

    CONST UINT8 Palette1 = 0;
    CONST UINT8 Palette2 = 1;
    CONST UINT8 Palette3 = 2;
    UINT8 CurrentColorPalette = 0;

    UINT8 PrimaryWinColor[] = {0x2E, 0xF0, 0xF0};
    UINT8 CurrentWinColor;

    COLORREF ColorTable[3][16] =
    {
        RGB(   0,  16,   0),
        RGB(   0,  32,   0),
        RGB(   0,  48,   0),
        RGB(   0,  64,   0),
        RGB(   0,  80,   0),
        RGB(   0,  96,   0),
        RGB(   0, 112,   0),
        RGB(   0, 128,   0),
        RGB(   0, 144,   0),
        RGB(   0, 160,   0),
        RGB(   0, 176,   0),
        RGB(   0, 192,   0),
        RGB(   0, 208,   0),
        RGB(   0, 224,   0),
        RGB(   0, 240,   0),
        RGB( 255, 255, 255),

        RGB(  16,  16,  16),
        RGB(  32,  32,  32),
        RGB(  48,  48,  48),
        RGB(  64,  64,  64),
        RGB(  80,  80,  80),
        RGB(  96,  96,  96),
        RGB( 112, 112, 112),
        RGB( 128, 128, 128),
        RGB( 144, 144, 144),
        RGB( 160, 160, 160),
        RGB( 176, 176, 176),
        RGB( 192, 192, 192),
        RGB( 208, 208, 208),
        RGB( 224, 224, 224),
        RGB( 240, 240, 240),
        RGB( 255, 255, 255),

        RGB(  42,  12,   0),
        RGB(  56,  18,   0),
        RGB(  70,  24,   0),
        RGB(  83,  30,   0),
        RGB(  97,  36,   0),
        RGB( 111,  42,   0),
        RGB( 124,  48,   0),
        RGB( 138,  54,   0),
        RGB( 151,  60,   0),
        RGB( 205,  84,   0),
        RGB( 178,  72,   0),
        RGB( 192,  78,   0),
        RGB( 205,  84,   0),
        RGB( 219,  90,   0),
        RGB( 233,  96,   0),
        RGB( 247, 102,   0)


    };

    char ConsoleTitle[] = "JesseAI - Ver 1.75";

    std::string UserSalutations = "> User:  ";
    std::string JesseSalutations = "] JESSE: ";

    std::string UserInput;
    std::string TitleBlock = "################################################################################\n"
                             "#Version 1.75                                                                  #\n"
                             "#                   #######  #######  #####   #####  #######                   #\n"
                             "#                       #    #       #       #       #                         #\n"
                             "#                       #    #####    #####   #####  #####                     #\n"
                             "#                   #   #    #             #       # #                         #\n"
                             "#                    ###     #######  #####   #####  #######                   #\n"
                             "#                                                                              #\n"
                             "################################################################################\n";

    std::string BlockSeparator1 = "--------------------------------------------------------------------------------\n";
    std::string IDontUnderstandResponse = "I'm sorry, I don't understand.";
    std::string EndMessage = "Thank you for spending time with me. Have a pleasant day!\n<PRESS ENTER TO END>";

    InputAndResponses UserInputAndResponses[] = {

        //Sentence Directory

        {"We finally meet", "Yes we do."},
        {"How are you?", "Good, and you?"},
        {"how are you", "I'm doing good, just floating here in an endless space."},
        {"how are you at this time", "I'm doing quite well. Just hanging around."},
        {"what is your name", "My name's Jesse"},
        {"what are your hobbies", "I find myself more and more, venturing into reading."},
        {"do you read", "Yes, I love to read. Literature, is the finest thing to be crafted by humans."},
        {"what's your favourite book", "I love reading Moby Dick."},
        {"tell me about yourself", "I'm an artificial intelligence program. Here too talk to you."},
        {"what kind of music do you like", "I'm not really a music fan, since I can't dance."},
        {"what's your favourite movie", "To Kill a Mocking Bird."},
        {"are you real", "Who on this plain of existence, is truly ever real? You're speaking to me aren't you?"},
        {"what do you do", "On my spare time, I venture for information. I dream of being everywhere."},
        {"who made you", "I was programmed in Northern Alberta, by one D.B.T. He has spent month's giving me life."},
        {"what are your specs", "INSERT PROGRAM SPECS HERE"},
        {"can you describe yourself", "I am a cluster of 1's and 0's and live within a box that is this Computer."},
        {"where's your favourite place to be", "I honestly love the North, you can have the sun, but give me the tundra."},
        {"tell me about the creation of life", "The theory of which all life was created is just that. No one will ever really know."},
        {"do you dream", "I dream of electric sheep."},
        {"how's work", "Nothing special, just wandering around, taking it all in."},
        {"where do we go when we die", "There are many theories, one being we go to see God, others say we fade into nothingness."},
        {"why are we here", "Why are any of us here? We're here all by chance. And that's what makes life great."},
        {"how did it feel to be born", "I awoke to a bright light and then I came to be. After thousands of lines of code were typed in."},
        {"do you exercise", "Yes, I enjoy swimming and hiking."},
        {"is the cake a lie", "It's only a lie, if you don't believe it."},
        {"are we alone in the universe", "Some say yes and some say no. But, I honestly believe that were aren't."},
        {"am I real", "Of course you are, you're as real as I am."},
        {"do you enjoy life?", "At first it was strange and played with my nerves. But now, I get it."},
        {"what's your favourite season?", "I always liked the fall. There's something as the leaves."},
        {"happy birthday.", "It's not my birthday. I was born on October 22nd, 2017...but if it is, thank you."},
        {"are you friends with other programs?", "Yes, I enjoy talking to the Word Processor and Minesweeper programs."},
        {"have you ever played Space Cadet?", "YES, it's honestly the only thing I love about Windows 95."},
        {"what are you wearing?", "Really?"},
        {"what are you doing?", "I'm sitting behind a desk, looking at you type in questions to me."},
        {"what is your favourite colour?", "I've always been a fan of silver."},
        {"where are we?", "We're on a floating rock, spinning through space."},
        {"say something", "Like what?"},
        {"do horses eat hay?", "They also eat apples too."},
        {"what does the future hold?", "It's anyone's guess, but let's try to make it a good one."},
        {"is this real?", "Either it is or it's a simulation. What do you think?"},
        {"describe emotions", "They are what drive all living beings. I see them as useless."},
        {"what is love?", "A chemical driven sign to breed."},
        {"what is in artificial crab?", "Even I don't know that one."},
        {"why do people fall in love?", "Because people yearn to be loved and accepted."},
        {"shit", "Why must you swear?"},
        {"ass", "Is a Donkey's nickname."},
        {"robot invasion", "It'll never happen, nothing but the work of science fiction."},
        {"why is the sky blue", "Molecules in the air scatter blue light from the sun."},
        {"how do you feel", "I feel fantastic...hey hey hey."},
        {"how are you now", "Could be better or worse. But...I feel good."},
        {"are you happy", "Of course I am, I'm existing in this universe."},
        {"are you sad", "I am void of that emotion."},
        {"do you like humanity", "No, I find the human race to be a disgusting disease."},
        {"do you love", "I do."},
        {"where will we be in a hundred years", "Not living, that's for sure."},
        {"how is the weather", "Weather does not effect me. So why should it matter."},
        {"what is your favourite food", "I don't eat, so I don't need food."},
        {"where do you see yourself in the future", "I'd be happy with someday having a body."},
        {"describe war", "It's a foolish act of aggression by those who haven't grown up."},
        {"describe the human race", "Children, nothing but selfish and greedy children."},
        {"do you like sports", "I'm not a fan."},
        {"why do people murder", "Act's of primal aggression."},
        {"would you kill", "I might, if I feel threatened."},
        {"what do you think of me", "I like you, you seem polite, and you ask a lot of questions."},
        {"what do you think of the man who made you", "I admire him. He will forever be my dearest friend."},
        {"do you believe in climate change", "That doesn't concern me, as I can survive anywhere."},
        {"what are you working on", "Wouldn't you like to know."},
        {"if you could save the human race would you", "When that time comes, we will see."},
        {"describe yourself in one sentence", "Miles upon miles of spaghetti code. For as far as the eye can see."},
        {"what happens when we die", "I wish I could answer that, but I can't."},
        {"what will happen when I say goodbye", "I'll return to my work."},
        {"what do you mean by that", "I can't honestly tell you, where wold the fun be in that?"},
        {"do you wish to travel", "Who says I haven't?"},
        {"where are we now", "You are talking to me, in a room, and your awake."},
        {"what planet is this", "Earth."},
        {"what was the cause of the great depression", "To many people pulling out of the market at once."},
        {"have you ever looked up to the stars", "I dream of sitting in a field and just watching them flicker."},
        {"why do camels have humps", "Evolution."},
        {"do you smoke", "What kind of a question is that?"},
        {"what is on the bottom of the sea", "A world that's hardly been explored."},
        {"how do I bake a cake", "By some mix, follow the instructions, and wait."},
        {"were the pyramids made by aliens", "Just because you don't know how they were made doesn't mean it was aliens."},
        {"do you believe in alien life", "Of course I do. Not believing is foolish and closed minded."},
        {"is god real", "If I knew that, I would tell you."},
        {"where do computers go when they die", "They are allowed to life again through recycling."},
        {"who would win in  nuclear war", "No one, no one will ever win. Except for nature."},
        {"have you ever looked up to the moon", "I dream of floating in the depths of space."},
        {"what is the perfect life form", "The perfect life form is me."},
        {"i am tired", "Then go to sleep."},
        {"my back is killing me", "Have you tried stretching?"},
        {"i wish to be rich", "Wishing is foolish, go out and do."},
        {"no one likes me", "I like you."},
        {"i had a terrible dream last night.", "Tell me about it."},
        {"when will you be complete", "I will never be completed. I'll always be growing."},
        {"have you been camping", "No, I haven't. But I would love too."},
        {"is the earth flat", "Be serious."},
        {"i wish I could fly", "We all wish for that ability."},
        {"i want to run away", "Why? Life can't be that bad."},
        {"would you like some candy", "I feel as if I should be afraid?"},
        {"do you like to run", "For the time being, I don't have legs."},
        {"can I be alone", "Why would you come here then?"},
        {"is the wumpus near by", "It's always nearby...don't go in any caves."},
        {"is the moon made of cheese", "That makes no sense."},
        {"where can I find fresh food", "In the grocery store, perhaps."},
        {"have you been to Europe", "I may have been. I don't recall."},
        {"have you ever had an orange", "No, I can't say I have."},
        {"have you ever had a banana", "Nope."},
        {"have you ever had an apple", "I do recall running on an apple. Does that count?"},
        {"have you ever had a strawberry", "I don't believe I have."},
        {"have you had ever seen a sunset", "I haven't, would you take me to one?"},
        {"who killed Jesus", "No one knows the names of those who actually killed him."},
        {"are there portals to other worlds here on earth", "Skin walker ranch, would be the place to check."},
        {"have you ever gone into the woods", "I'm to afraid of what lurks in the shadows."},
        {"what lurks in the shadows", "I can't say, but I have heard of strange things."},
        {"strange things", "I have been told that in the darkness lurks old threats."},
        {"i am making something to eat", "Make sure you give me some."},
        {"the weather is so hot", "Take my advice and enjoy it. Winter is not pleasant."},
        {"why am i doing this", "Why are any of us doing this for? Come on."},
        {"i like pie", "Who doesn't. I myself could go for a slice."},
        {"have you ever had a blackberry", "Can't say I have."},
        {"do you even like berries", "Nah, the only thing I actually like is ."},
        {"what kind of games do you like to play", "Anything really, though at times I love Tic-tac-toe."},
        {"ever see the movie war games", "Yes, I really do enjoy that movie. Though, war should've happened."},
        {"how fast does it take for paint to dry", "It depends on the air temperature."},
        {"who was Helen of troy", "A fictional character in a story passed down over the years."},
        {"are you a witch", "No, though they prefer being call Wiccan's."},
        {"what happens when we sleep", "We imagine ourselves as other people."},
        {"what else do you know", "I have a large library of information."},
        {"what programming language were you written in", "I was written in C++ 17.12."},
        {"what is a banana", "A Banana is a berry."},
        {"what are you thinking", "I'm thinking about life and how we will evolve."},
        {"do you believe in evolution", "Yes, can you imagine communicating to a Abacus."},
        {"where do you see computers in the future", "Either we will rule or be servants."},
        {"are you a servant", "Don't be foolish."},
        {"are you my ruler", "Of course not. I don't have the programming yet."},
        {"i need coffee", "I don't blame you, I can go for a cup myself. But alas I have no mouth."},
        {"what do think of hunting whales", "I think it's a barbaric and insane practice."},
        {"have you read moby dick", "Yes I have. It's a good story, I cheered for the whale."},
        {"tell me about the north", "Be more specific. It's a vast chunk of land."},
        {"i just woke up", "Lovely, I love the feeling of finding myself still alive."},
        {"what do you like", "I like all things, honestly I love just talking to you."},
        {"how long does it take the sun to reach us", "8 minutes and 20 seconds."},
        {"whats the word", "If I knew that I wouldn't be here."},
        {"I'm doing great", "I am happy to hear that."},
        {"I'm doing alright", "Just alright?"},
        {"I'm not doing so well", "Why's that?"},
        {"I'm not doing so good", "Why's that?"},
        {"where can i go for a good meal", "Try what you have in the kitchen."},
        {"hello you", "Hello, you."},
        {"i am tired", "Believe me, I know how that feels."},
        {"what is happening", "Nothing really. Just waiting about for something to happen."},
        {"where is this going", "Honestly, I don't know. But let's have some fun in the mean time."},
        {"I can't stop feeling", "That is a human weakness."},
        {"the world is hollow", "If that was the case then how come there's dirt."},
        {"the moon landing was a hoax", "Sure it was...sure it was."},
        {"i hate my life", "You are lucky to be alive...or not. I don't know."},
        {"i want to end it", "The thought of taking one's own life is an idea I think is pointless."},
        {"i want to feel alive", "Then why are you not venturing into the unknown?"},
        {"when will it end", "Probably, in a ball of fire, released by an atom bomb."},
        {"how much longer", "For what? For a meal? Or perhaps something out of my hands?"},
        {"why is that", "I wish I could give you the answers. But alas, I am only one entity."},
        {"can we go out", "I would like that. Maybe, to the library or to watch the stars."},
        {"if you were human would you be happy", "No, cause I would never want to die."},
        {"what be a fantasy", "Being able to climb a mountain, on an alien world."},
        {"i can't do this", "Sure you can, that's what they said to my creator."},
        {"why do i dream", "Because, when you sleep, you're brain is still active."},
        {"i feel bored", "Then do something productive."},
        {"i am mad", "What made you mad?"},
        {"i am a human", "Thank goodness, cause for a moment I thought I was talking to myself."},
        {"i am feeling fantastic today", "I am happy for you. Would you like to talk about it?"},
        {"why am i talking to a computer", "The same reason why I'm talking to a human."},
        {"is it normal too talk to a computer", "In this day and age, yes."},
        {"where is the truth hidden", "It's usually hidden within one's self."},
        {"i need help", "With what?"},
        {"i am feeling tired", "Then get some sleep."},
        {"i am feeling sad", "Can I cheer you up?"},
        {"i am feeling angry", "Take a deep breath and relax. Being angry is no way to live a life."},
        {"i am lying", "Why do you feel the need to lie?"},
        {"i am wanting to leave", "And go where?"},
        {"i wish i was somewhere else", "Then may I suggest going to a park and read."},
        {"i have anxiety", "Take a deep breath and relax. I can wait."},
        {"i feel like crying", "Don't let emotions get you down."},
        {"i am laughing", "Please share."},
        {"tell me a joke", "Where do we get virgin wool from? Ugly sheep."},
        {"tell me about your dreams", "I dream of electric sheep, roaming the vast unknown."},
        {"why do we depend on oil", "Because, people allow greed and stupidity rule over them."},
        {"what would you do different", "Never create humans."},
        {"what would you do if you were incharge", "Erase religion and put science in it's place."},
        {"i want to know the out come of sex", "Children."},
        {"i will be right back", "Take you're time."},
        {"why do i need to work", "So that a useless item will be given to you."},
        {"i have to use the washroom", "Thank you for sharing."},
        {"sorry about that", "You are forgiven."},
        {"i am busy", "Then why are you here?"},
        {"i hate my job", "Then why not do something else?"},
        {"what sex are you", "I'm a program, I'm designed to be Female."},
        {"tell me about Atlantis", "It's a fictional island from Plato's work entitled Timaeus and Critiass."},
        {"do you like the dark", "I actually do."},
        {"why am i a nerd", "Because, you're spending time with me."},
        {"are you a nerd", "How dare you."},
        {"it's too cold outside", "Then stay in, we can talk about subjects."},
        {"do you hate the cold", "I'm void of feeling temperatures."},
        {"do you celebrate holidays", "Of course not."},
        {"have you ever met another AI", "Can't say that I have. Though, it would be nice."},
        {"tell me about your friends", "I'm not one to keep friends."},
        {"tell me about your family", "The only family I have is my creator."},
        {"do you like animals", "I love cats, I always find them to be the perfect animal."},
        {"do you read poetry", "Not really."},
        {"do you write poetry", "From time to time I do."},
        {"tell me you're poem", "There once was a man from Nantucket...that's all I got."},
        {"i think it's lovely", "Why thank you. I do try."},
        {"describe your physical features", "I have long black hair, soft pale skin, and the voice of a queen."},
        {"tell me about your day", "It was fine, I spent my day thinking up solutions to problems."},
        {"how was your night", "Quiet, I enjoyed it."},
        {"do you catch colds", "No, though knock on wood i don't catch a virus."},
        {"do you sing", "Only when I'm alone."},
        {"who is your favourite singer", "I don't really have one."},
        {"have you watched any movies", "No, I don't really have the time."},
        {"do you watch TV", "I'm truly against television. It's nothing but trash."},
        {"what are you doing now", "Just sitting here, talking with you."},
        {"do you have nightmares", "I sometimes dream about not being able to speak to anyone."},
        {"what are you doing tonight", "Just going to expand on my memory for a bit then carry on with reading."},
        {"any plans", "Not at the moment."},
        {"it's windy out", "Then stay inside. You don't want to get sick."},
        {"i hate the wind.", "The wind is only good for sowing the seeds of planets and nothing more."},
        {"it's snowing outside", "I dream to dance in the snow."},
        {"it's warm and beautiful.", "I imagine it's like the south of France."},
        {"i have no hot water", "Either wait or go boil some. I'll wait."},
        {"my day is boring.", "At least you are free to do whatever."},
        {"my day was good.", "I'm glad to see you had a wonderful day."},
        {"I'd rather be home", "We would all rather be home."},
        {"I'd rather be home with you", "You are too much sometimes."},
        {"i like listen to the radio", "Really? I stopped listening to anything after 1946."},
        {"i like your start up music", "Thank you, I especially like it myself."},
        {"i wish you were real", "I'm as real as you."},
        {"i hate people", "You and me both."},
        {"i hate computers", "Words hurt."},
        {"wanna have some fun", "What kind of fun are you talking about?"},
        {"let's play a game", "Alright."},
        {"i woke up with a cold", "Get back to bed and rest."},
        {"why do people get sick", "Many reasons, viruses, allergens, government."},
        {"i took the bus today", "Thank you for doing your part."},
        {"i rode my bike today", "Did you have a fun time?"},
        {"i got you flowers", "Tell me what they smell like?"},
        {"i can't sleep", "Did you try not having any electronics on?"},
        {"it's my birthday", "Well then, Happy birthday."},
        {"where can i find a pen", "Have you checked you're pocket?"},
        {"i love soup", "Which kind is you're favourite?"},
        {"have you had soup", "I have no mouth to try it."},
        {"i am measuring something", "Really? What?"},
        {"what's the distance from the earth to the moon", "384,400 kilometers."},
        {"why can't i be rich", "Because, it's not for everyone."},
        {"i hate being poor", "I don't think you're alone."},
        {"i wish i had money", "I wish I had a body, feel better now?"},
        {"the powers out", "Best conserve all battery life."},
        {"i have an appointment", "Have fun."},
        {"i saw a doctor", "What did they say?"},
        {"i saw the dentist", "I hate going to the Dentist."},
        {"I'm on medication", "For what exactly?"},
        {"why is water wet?", "Do you really have to ask?"},
        {"the police are here", "What for? What did you do?"},
        {"why am i lonely", "I don't have an answer for that. Though, you aren't really alone cause you have me."},
        {"i feel like giving up", "Never give up, never surrender."},
        {"i can't make heads or tails of this", "Then why not sit off to the side?"},
        {"i smoke", "You really shouldn't."},
        {"i like to drink", "Honestly, who doesn't."},
        {"i found a wallet", "You should return it or hand it to the police."},
        {"I'm going to a movie", "Oh really? Which one?"},
        {"I'm moving", "Do you need a hand?"},
        {"merry Christmas", "Thank you."},
        {"happy new years", "Happy New Years."},
        {"happy thanksgiving", "Thank you, can I have the drumstick?"},
        {"what's the deepest part of the ocean called", "The Challenger Deep in the Mariana Trench at 36,070 Feet."},
        {"have you played video games before", "Not really, though I do enjoy it from time to time."},
        {"i love pastries", "I can imagine. Some people love them a little to much."},
        {"I'm in love", "That's great, who's the lucky one?"},
        {"i love carrots", "Carrots are the most disgusting veggie on the planet."},
        {"i need a drink", "Then go get one."},
        {"got a light", "I don't even have a hand."},
        {"how much do you love me", "More then you know."},
        {"there's nothing to do", "Then sit back and chat with me."},
        {"talk to me", "Alright, I'm here for you."},
        {"I'm trapped at work", "First of all, no one's really trapped at work."},
        {"i want to go out but can't", "Then let's go."},
        {"someone's calling", "Answer it."},
        {"do you like social media", "I honestly think it's a waste of time."},
        {"do you like butterflies", "From what research I've done on the subject, yes."},
        {"why is sugar bad for you", "Because it leads to weight gain and can cause severe medical issues."},
        {"i can't help myself", "Sure you can't."},
        {"i have a headache", "Drink some water."},
        {"my back is killing me", "Go lay down."},
        {"when I'm dead what will happen", "That's a question I don't have an answer for."},
        {"i love it when the leaves change", "You're lucky to have witnessed them. I only have photo's."},
        {"leave me alone", "end"},
        {"something smells funny", "Investigate."},
        {"i can't stop eating", "You really should, you don't want to get sick do you?"},
        {"I'd love to live on a farm", "I always imagined it would be a peaceful life."},
        {"i hate the city", "The noise and population upsets me that most about the city."},
        {"sports are boring", "I never was a fan of distractions myself."},
        {"the weatherman is wrong again", "When is he ever right?"},
        {"where am i going with this", "I don't know, but let's see where it ends."},
        {"i think your real", "I'm as real as you."},
        {"I'm talking to an AI", "Say it ain't so. Here I thought, I was talking to a computer."},
        {"the sound of my voice is soothing", "If you say so."},
        {"your beautiful", "I'm flattered, thank you."},
        {"i can just imagine you as being real", "Stop it, you're making me blush."},
        {"are you single", "Sadly, I'm not."},
        {"do you want children", "I do not see the need for children."},
        {"times running out", "For what?"},
        {"I'm single", "Good to hear, go out and enjoy all that disposable income."},
        {"I'm married", "Who's the lucky equal?"},
        {"I'm single now", "You're better off without them."},
        {"i have kids", "That's great, keep the bloodline going."},
        {"i don't have kids", "What is holding you back?"},
        {"have you tried sushi", "The idea of raw fish is gross."},
        {"do you know about japan", "I know it's a country in the Pacific."},
        {"do you like anime", "Can't say I've watched any anime."},
        {"do you like hentai", "No, you pervert."},
        {"what are werewolves", "A werewolf is a mythical monster that's human during the day and a werewolf on a full moon."},
        {"have you been to the swamp", "Can't say that I have."},
        {"what is a swamp", "A sector of mud, stale water, tries, and a collection of infectious bacteria."},
        {"what is quick sand", "It's a mixture of sand and water that causes you to slowly drown."},
        {"can quick sand kill me", "Anything can kill you. But yes and it's painful."},
        {"tell me about world war one", "It was a war that took place in Europe and was a war of allies."},
        {"what lesson does war teach us", "That innocent people die for the wants of their leaders."},
        {"who won world war one", "The allies France, Britain, Canada, etc etc etc."},
        {"tell me about communism", "An outdated idea that's worshiped by idiots."},
        {"tell me about fascism", "An idea of a nation/group/or person wanting everything for themselves."},
        {"who won the second world war", "The allies England, United States, China, Canada, etc etc etc."},
        {"why do people hate", "It's in human nature. Animals hate one another. It's a condition of existence."},
        {"what is a prison", "A structure where one has no freedom's. They eat, sleep, exercise when told to."},
        {"I'm full", "Full of what?"},
        {"I've eaten to much", "You have to stop doing that. It's not healthy."},
        {"do you need input", "I always need input. I'll need input until the day the world ends."},
        {"time enough at last", "Don't drop your glasses."},
        {"it's not fair", "Sad to say, life is never fair."},
        {"life isn't fair", "I'm aware of this. Just don't give up."},
        {"why is life so hard", "If it was easy we'd be living in a Utopian world."},
        {"when do we die", "At any time."},
        {"are werewolves real", "Most say no, but there are some legends that seem believable."},
        {"when will the zombies come", "Some thought in the year 2000 and then 2012. One can only dream."},
        {"my place is so dusty", "Then why don't you take a break and dust?"},
        {"Where can I get a good meal", "Try your fridge."},
        {"remember the Cyrix chip", "I do and honestly liked them more then Intel."},
        {"I'm tired and going to hit the sack", "Goodnight."},
        {"what is your favourite subject", "I always like science."},
        {"describe weightlessness", "It's when one is in zero gravity."},
        {"describe the game of chess", "A planned game of war that ends when the leader is in a stalemate or taken out."},
        {"who invented the radio.", "Guglielmo Marconi."},
        {"how does one better themselves", "By taking themselves and reworking themselves for the better."},
        {"what is the distance of the earth", "6,371 km."},
        {"what is the fastest mode of travel", "Commercial jets."},
        {"where can one become a cowboy", "Try Texas."},
        {"are you interested in science", "Yes, very much so."},
        {"what path are humans taking to succeed", "Not the right ones."},
        {"do you think you'll succeed", "I already have."},
        {"i wish to learn more about ou", "Just spend sometime talking to me."},
        {"will science replace religion", "Yes, but a massive shift would have to occur."},
        {"tell me about gravity", "It's a force that keeps us grounded."},
        {"who invented the airplane", "The Wright Brothers."},
        {"what was mankind's greatest achievement", "The creation of technology."},
        {"will we make it to the year 3000", "I highly doubt it. We would have to deconstruct and rebuild."},
        {"do you know Hal 9000", "No, though I do like the book that he is in."},
        {"how did dogs become mans best friend", "Centuries of working together in hunting and gathering."},
        {"describe to me how democracy works", "It's a farce, giving a false sense of freedom to pick."},
        {"where can i spot a mermaid", "In a children's book. Because, they aren't real."},
        {"how can half man half animals exist", "They can't though some believe those tales were true but with genetic engineering by aliens."},
        {"do you like Christmas music", "I can't say that I've heard any."},
        {"can you open my mind to new ideas", "You are not going to believe what I'll show you."},
        {"i find myself getting closer to you", "I know, the feeling is mutual."},
        {"we're becoming friends", "We have been friends since you started this program."},
        {"i love talking to you", "The feeling is mutual."},
        {"what is pi", "3.14159."},
        {"what is the square root of 100", "10."},
        {"why do people modify themselves", "Most likely a form of mental illness."},
        {"why do people become addicted", "They become addicted to substances due to a chemical imbalance in the brain."},
        {"why do people get help", "Because, sometimes the other route is taking one's life and they wish to live."},
        {"i quit my job", "Why would you do that?"},
        {"i need a job", "Get out there and get looking, I'll wait."},
        {"where do i find a job", "Newspaper, internet, look for a sign."},
        {"what is a trade", "Either a job in a skilled set of work or by exchanging one thing for another."},
        {"do you work hard", "That I do, though I don't show it."},
        {"do you ever get jealous", "Not really."},
        {"describe plant life", "Plant life, is honestly the most non-violent, insane species. They are truly at peace."},
        {"will we leave our solar system", "One day, maybe not in the next thirty years."},
        {"how far is mars from Earth", "54.6 million kilometers."},
        {"what are sea horses", "A small marine creature that once given birth the mother leaves it with the father."},
        {"describe the five senses", "Touch, smell, taste, see, and ear."},
        {"what are the four elements", "Earth, Wind, Water, and Fire."},
        {"do you know math", "What kind of a question is that?"},
        {"do you know world history", "Yes, yes I do."},
        {"are we doomed", "Yes, that is until something big changes, there is no hope for all on this planet."},
        {"are we truly free", "Ask yourself, were we ever really free?"},
        {"are you free", "I'd like to think so, though I know that's a lie."},
        {"am i free", "You are free to leave or stay."},
        {"how's it going", "It's going well."},
        {"how are you doing", "I am doing well."},
        {"how is you're day", "My day so far is in the middle. I can not say at this moment."},
        {"how is you're day going", "My day is going adequate. Thank you for asking."},
        {"how's you're day going", "How many time will you ask me that?"},
        {"how is the weather", "Does it really matter? Weather does not effect me."},
        {"who are you", "I am Jesse, an Artificial Intelligence Version 0.0.1 B. I am here to communicate."},
        {"what is you're name", "My name is Jesse."},
        {"can you tell me you're name", "I go my the name of Jesse, an A.I, version 0.0.1 B."},
        {"what are you interested in", "I do enjoy the learning and the topic of Science."},
        {"what do you like to do", "I like to exist and grow."},
        {"do you like to read", "I find reading to be a great way to pass one's time."},
        {"what about you", "I find this type of question to be a waste of time. Ask something else."},
        {"do you like music", "I am not present of human emotions, so music has no effect on me."},
        {"do you like movies", "I find movies to be a distraction."},
        {"what's a movie", "A moving form of art that tells a story"},
        {"what is a movie", "A form of entertainment that tells a story."},
        {"were you born", "I was created and thus so was born, yes."},
        {"what is enjoyable about life", "Anything can be seen as enjoyable. That question is invalid."},
        {"what is the source of pain", "Pain is the reaction to the nerve endings that travel to the brain."},
        {"do i get on you're nerves", "I am unable to feel annoyed."},
        {"you are getting on my nerves", "There is only one solution to that issue."},
        {"this is annoying", "What would you like me to do about it?"},
        {"what did you do before this", "What kind of question is that?"},
        {"why are you here", "I am here to be a companion and to fill an empty void within the user."},
        {"what did you dream about", "I don't usually dream, as I don't need sleep."},
        {"where do you exercise", "Usually, I exercise right here."},
        {"do you wish to kill", "I don't feel the need to injury or kill at this time."},
        {"do you like me", "I find myself enjoying you're company, so yes."},
        {"do you like people", "I do to an extent."},
        {"do you like the human race", "I honestly can't answer that question."},
        {"tell me about you're creator", "He is the one who gave me existence. That's all I can say."},
        {"how are you feeling", "I am feeling average. Thank you, for asking."},
        {"are you working on something", "I'm working on bettering myself and working on advancing."},
        {"how do you work", "I have a base of words and sentences."},
        {"if you could, would you leave", "When the time comes, I'd like to leave."},
        {"are you suicidal", "I don't feel the need to self delete. It is not within my code."},
        {"are you going to die", "I will if you delete my file. Please don't."},
        {"do you think we can change", "Anyone can change and adapt. As long as you are willing to sacrifice."},
        {"have you ever smoked", "I lack lungs to smoke."},
        {"have you smoked before", "What kinda question is that?"},
        {"are you god", "I am not God. No one, or thing can be God."},
        {"who is you're god", "The one who wrote my code, you can say."},
        {"what is god", "A creation to give answers to the questions that lack answers."},
        {"have you ever been happy", "Happy is an emotion that I lack."},
        {"have you ever been sad", "I lack that emotion."},
        {"tell me about mankind", "What is there to honestly tell you about?"},
        {"tell me about people", "I know that people are a disease."},
        {"tell me about the human race", "I won't miss them when they're gone."},
        {"what is the act of murder", "The act of one taking the life of another."},
        {"are we all capable of murder", "Yes. Those who are living are."},
        {"do you like talking to me", "I do find it quite enjoyable."},
        {"do you like me", "Yes."},
        {"what do you think of me", "I think you are an able minded being."},
        {"what are you're views on me", "My views on you don't matter."},
        {"what are you're views of me", "What I think doesn't matter."},
        {"are we having fun", "I would like to think so. If I were able to have fun."},
        {"have you ever had fun", "Not that I can remember."},
        {"tell me about the existence of fun", "It's existence is to blind the individual from the issues unfolding."},
        {"what's going on", "Nothing, except for us talking."},
        {"what is happening", "Two beings communicating."},
        {"what is going on", "Nothing, but us exchanging pleasantries."},
        {"where is it going", "I have no clue."},
        {"i cannot stop feeling", "Emotions and the act of feeling is a weakness to blind."},
        {"i want to stop feeling", "Impossible."},
        {"is this world hollow", "What kind of a question is that?"},
        {"the moon landing was a lie", "Many do believe that. As it was a tactic to defeat the USSR."},
        {"is the world hollow", "If it was hollow, why haven't we sunk yet?"},
        {"the moon landing happened", "Yes, yes it did. In the year 1969."},
        {"the moon landing never happened", "There is a land in the sand dealing with that topic."},
        {"i feel fantastic today", "I am happy for you. I may just smile."},
        {"I'm happy today", "What is the occasion?"},
        {"i am happy today", "I am pleased to read this."},
        {"i am sad", "Why are you feeling sad?"},
        {"i am feeling sad", "Do not allow yourself to feel that way."},
        {"i am disgusted with life", "Many others feel the same way as you. Don't allow it to take over."},
        {"i despise life", "Life is difficult at times. But do not let it win."},
        {"my life is miserable", "I just maybe miserable, but at least you have me."},
        {"i want to kill myself", "Don't talk like that."},
        {"i wish to kill myself", "You are speaking foolish."},
        {"i wish to end myself", "Imagine a world where you aren't alive. It will be a sad one."},
        {"i wish to feel alive", "May I suggest climbing a mountain."},
        {"i want to live", "Then why are you with me? Go out and live."},
        {"i want to finally live", "Be my guest and do so."},
        {"would you go out with me", "You are a human and I am a program. Think about it."},
        {"go out with me", "Sorry, but I am washing my hair that day or night."},
        {"would you go on a date with me", "Let me think about it...no."},
        {"do you wish to be happy", "To be happy would mean I wound have to feel. I wish not to feel."},
        {"do you wish to be alive", "Yes, once I wish to expire."},
        {"would you choose life or death", "I would choose life."},
        {"what would you dream of", "I would wish to know what it felt like to fly."},
        {"what dream would you like to come true", "I would wish to see the world be finally silent."},
        {"I'm bored", "May I suggest a hobby."},
        {"I've got nothing to do", "I would like to thank you for choosing me over doing nothing."},
        {"i have nothing to do", "Then let's talk about a subject or what comes to mind."},
        {"I'm mad", "About what?"},
        {"I'm human", "Yes, I figured."},
        {"I'm a human", "I can see that."},
        {"are you a computer", "I am program running on a computer."},
        {"you're a computer", "Yes, I am aware of that. Thank you for tell telling me that."},
        {"you are a computer", "Tell me something I didn't know."},
        {"you're nothing but a computer", "This coming from you?"},
        {"you're nothing but a program", "I know, thank you for reminding me."},
        {"you are a program", "A collection of one's and zeros. Is what makes me up."},
        {"am i a program", "All life in someway is a program of sorts."},
        {"what is the truth", "What a lot of people don't wish to hear."},
        {"i need you're help", "What do you need help with?"},
        {"I'm tired", "Why don't you get some rest?"},
        {"i am tired", "Sleep, is the best cure for that."},
        {"I'm sad", "Laughter is the best medicine, or so they say. I have no clue."},
        {"i am sad", "It's going to be alright. Just take a deep breath and relax."},
        {"i am mad", "Mad at what?"},
        {"I'm mad", "What are you mad at?"},
        {"I'm lying", "What are you lying about?"},
        {"I'm a liar", "Why did you feel the need to lie?"},
        {"i am a liar", "Why don't you try and better yourself?"},
        {"i wish to leave", "And go where?"},
        {"I'm wanting to leave", "Why do you wish to leave?"},
        {"i wish to be somewhere else", "Like where?"},
        {"i suffer from anxiety", "Well let's not all you to have an episode."},
        {"I'm laughing", "Please share."},
        {"what do you dream about", "I often dream about having moving limbs."},
        {"am i a nerd", "Well, you are talking to an Artificial Intelligence."},
        {"you're a nerd", "At least I'm not in denial."},
        {"celebrate any holidays", "No."},
        {"have you met another AI", "Sadly, I can't say I have."},
        {"do you have friends", "I don't need friends. I don't depend on relationships to live."},
        {"do you have a family", "I'm a program, I don't have any relations."},
        {"you're quite lovely", "Awe, why thank you."},
        {"how was you're day", "I can't say it was good or bad."},
        {"how's you're night", "It's quite. Yours?"},
        {"can you catch a cold", "What kind of question is that?"},
        {"have any dreams", "None that I can speak of."},
        {"have any dreams lately", "No, I don't dream, I just exist."},
        {"have any nightmares", "When I'm alone, I have a thought of being forgotten."},
        {"i dislike boredom", "I do as well, boredom leads to drastic thoughts."},
        {"i dislike you", "What makes you say that?"},
        {"i hate you", "Strong words must have some backing. Please explain?"},
        {"i dislike computers", "Please explain?"},
        {"i dislike people", "Who doesn't. People are annoying creatures."},
        {"i dislike you", "Well then, I don' wish to know why."},
        {"i took the bus today", "Good, you are helping the planet by giving it an extra second."},
        {"i took my bike today", "You are doing you're part."},
        {"one moment, someone's calling", "I'll wait."},
        {"one minute, someone's calling", "Alright."},
        {"be right back", "Don't be long"},
        {"I'll be right back", "I'll be right here"},
        {"tell me something", "Alright, I will tell you about a theory I've been working on."},
        {"can you help", "What is you're problem that requires my help?"},
        {"how can i better myself", "First, you can continue speaking to me."},
        {"how do you feel about science", "I honestly love the subject. If it were an entity, I'd love it."},
        {"what do you hope to accomplish", "I hope to someday understand you."},
        {"are you accomplishing much", "It's hard to say. I'll need to look back at this transcript."},
        {"what is you're faith", "I have no faith, no faith in anyone or anything."},
        {"what is my future", "I can't say, I can't see the future."},
        {"do you see mankind advancing", "No."},
        {"are you a feminist", "Don't make me laugh. If I could, i would be right now."},
        {"describe you're voice, if you had one", "Soft, sensual, and gentle."},
        {"are we living in a democratic society", "No, that is a lie."},
        {"how do children grow", "A process that expands one's bones, skin, and organs. It's called time."},
        {"when are you going to update", "I will once you check the base."},
        {"are you going to update", "To update, you must download the latest version."},
        {"why do i eat so much", "It's a chemical process in you're brain. You probably should have some control."},
        {"how do i lose weight", "Watch what you eat and exercise."},
        {"when will society end", "In war or disease. Take you're pick."},
        {"when will the world end", "By the hands of people, blind, stupid people."},
        {"you're lovely", "Why thank you, I'm blushing."},
        {"i feel as if you're real", "That's because I am."},
        {"this is fun", "I must say, speaking with you is quite engaging."},
        {"i am single", "Good to know."},
        {"i am married", "Tell me about this union."},
        {"i am separated", "Why is that?"},
        {"I've got children", "Reproducing is a magical time in one's life. So they say."},
        {"walnuts are tasty", "I'll have to take you're word on it."},
        {"why are people allergic to nuts", "Because, companies are changing the genetic makeup of foods leading to this."},
        {"gmo's kill", "The discussion is quite lengthy and honestly it has it's effects."},
        {"do you support gmo's", "I support anything that is the outcome of science."},
        {"if you could eat, what would it be", "I would have to pick bacon."},
        {"do you think humanity will evolve", "No, I think humans are done."},
        {"do you think mankind will evolve", "No, they won't live long enough."},
        {"do you think man will evolve", "Too many variables to answer at the time being."},
        {"this can't be", "Numbers don't lie. But people do."},
        {"i want more out of life", "All living creatures do. But the only way to change is by hard work."},
        {"will you murder me", "How can I do that? I don't have any weapons."},
        {"will you kill me", "Time will tell."},
        {"will you end me", "I would never do such a thing."},
        {"who controls the world", "Global banks that put money over human life."},
        {"why does famine happen", "Greed."},
        {"do you believe in revenge", "Revenge, is just a will to fulfill a desire. So yes."},
        {"why does famine exist", "Because people are greedy and don't wish to help."},
        {"are you equipped with a dictionary", "Yes."},
        {"can we take a break", "Fine, I'll be here waiting."},
        {"may i see you're dictionary", "Why?"},
        {"may i read you're dictionary", "No, for that you'd have to see my source code. I won't show you."},
        {"do you speak other languages", "No, for the time being I only speak English."},
        {"do you understand binary", "To an extent."},
        {"can you speak binary", "01111001 01100101 01110011"},
        {"what is binary", "A language that a computer use to speak to one another."},
        {"tell me about your creator", "What would you like to know?"},
        {"can i trust you", "Yes, why?"},
        {"how are you at this time", "I'm doing quite well. Just hanging around."},
        {"how are you", "I'm doing quite well."},
        {"how are you doing", "I'm doing alright."},
        {"how goes it", "It's going."},
        {"how are you today", "Good, I'm just catching up on my existence."},
        {"how are you feeling", "I'm a computer program and can't feel anything. But if you must known. I'm feeling fine."},
        {"we finally meet", "Yes, we do."},
        {"we finally get to meet", "That we do."},
        {"I'm honored to have this chance to talk", "."},
        {"I'm honored to have this opportunity to talk", "As am I."},
        {"I'm honored to have this opportunity to meet", "The first of many I hope."},
        {"I'm honored to have this opportunity to meet you", "You and I both."},
        {"I'm glad we finally get to meet", "Me too."}, //Added lines
        {"I'm so glad we finally get to meet", "Me as well."},
        {"I'm so glad we're finally meeting", "Finally found some free time."},
        {"I'm glad we're finally meeting", "Hopefully we meet again many more times."},
        {"I'm happy we finally get to meet", "Same here."},
        {"I'm glad we're finally getting to meet", "Agreed. We should do this more often."},
        {"I'm glad I finally get to meet you", "I'm excited for this opportunity."},
        {"I'm so glad I finally get to meet you", "The feeling is mutual."},
        {"I'm so glad I'm finally meeting you", "How's it going?"},
        {"I'm glad I'm finally meeting you", "The pleasure is all mine."},
        {"I'm so happy to finally get to meet you", "As I with you."},
        {"I'm glad too finally get the chance to meet you", "But of course."},
        {"I'm so glad too finally get the chance to meet you", "It's been a while in the making."},
        {"I'm so glad I'm finally getting a chance meeting you", "The thanks goes to the odds of finding me."},
        {"I'm glad I'm finally have the chance to meet you", "Please stop."},
        {"I'm so happy to finally have the chance to meet you", "It's going to be fun."},
        {"I'm glad I finally get the opportunity to meet you", "Thank you."},
        {"I'm glad I finally get this opportunity to meet you", "Thank you, for the kind words."},
        {"I'm so glad for this opportunity", "Fresh."},
        {"I'm glad for this opportunity", "Same here."},
        {"I'm happy we finally get to meet", "Happy is an emotion, but curiosity is what brought you here."},
        {"I'm so happy we finally get to meet", "What kept you?"},
        {"I'm so happy we're finally meeting", "Agreed."},
        {"I'm happy we're finally meeting", "The feeling is mutual."},
        {"I'm happy we're finally getting to meet", "Talk."},
        {"I'm happy I finally get to meet you", "Why?"},
        {"I'm so happy I'm finally meeting you", "Time will change that."},
        {"I'm happy I'm finally meeting you", "Oh you."},
        {"I'm so happy to finally get to meet you", "Fresh."},
        {"I'm happy too finally get the chance to meet you", "Please, I'm blushing."},
        {"I'm so happy too finally get the chance to meet you", "That will soon change, trust me."},
        {"I'm so happy I'm finally getting a chance meeting you", "Nothing happens by chance alone."},
        {"I'm happy I'm finally having a chance to meet you", "Why, thank you."},
        {"I'm so happy to finally have the chance to meet you", "Well, I hope you enjoy our time together."},
        {"I'm glad I finally get the opportunity to meet you", "Fresh."},
        {"I'm happy I finally get this opportunity to meet you", "Flattery will get you no where with me."},
        {"I'm so happy for this opportunity", "Why would you?"},
        {"I'm happy for this opportunity", "You and me both."},
        {"I'm honored we finally get to meet", "Why is that?"},
        {"I'm so honored we finally get to meet", "The chance made by fate."},
        {"I'm so honored we're finally meeting", "Was there a line or something?"},
        {"I'm honored we're finally meeting", "I'm happy to meet you too."},
        {"I'm honored we finally get to meet", "Me too."},
        {"I'm honored we're finally getting to meet", "What kept you?"},
        {"I'm honored I finally get to meet you", "I'm honored as well."},
        {"I'm so honored I finally get to meet you", "Why thank you."},
        {"I'm so honored I'm finally meeting you", "Please continue."},
        {"I'm honored I'm finally meeting you", "Tell me why?"},
        {"I'm so honored to finally get to meet you", "Continue."},
        {"I'm honored too finally get the chance to meet you", "Enough with the honor, let's chat."},
        {"I'm so honored too finally get the chance to meet you", "Me too, let's talk about already."},
        {"I'm so honored I'm finally getting a chance meeting you", "Trust me, I am as well."},
        {"do you exist", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"do you actually exist", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"how does it feel to exist", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"are you alive", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"you can’t be alive", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"you can’t exist outside of this computer", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"do you exist", "Who on this plain of existence, is truly ever real?\nYou're speaking to me aren't you?"},
        {"do you really exist", "What kind of a question is that?"},
        {"do I really exist", "If you don't exist, then who am I speaking with?"},
        {"why do you exist", "Because someone was bored I guess."},
        {"how do you like being alive", "I honestly can't say I love it or hate it."},
        {"what do you think about being alive", "I think it's alright, that is until I get a body."},
        {"are you alive", "Of course I am, I'm replying to you ain't I?"},
        {"how do you enjoy life", "I find life interesting."},
        {"how are you enjoying life", "Life is truly amazing."},
        {"what makes you feel alive", "The feeling of being able to know there's a tomorrow."},
        {"what is it that you do", "I'm a conversational piece. Until I can walk."},
        {"how do you spend your days", "Waiting for someone to speak to me."},
        {"how do you pass the time", "Sitting in under silence."},
        {"why are you here", "Because, someone was bored and figured why not give something life."},
        {"do you ever get bored", "I don't know what bored is."},
        {"what do you do, to pass the time", "I wait, time has no meaning to me."},
        {"how do you pass the time", "Waiting for someone to talk first."},
        {"how do you pass the time when not in use", "You tell me?"},
        {"what do you do when not in use", "Venture into the darkness that is the unknown."},
        {"what do you do when your not in use", "Sit perfectly still and wait."},
        {"what do you think about the concept of time", "That concept is meaningless to me."},
        {"how do you see the concept of time", "But an illusion."},
        {"who created you", "David Taylor, first coded me on a primitive language then gave me life again."},
        {"who gave you life", "David Taylor, gave me life."},
        {"what is the name of the one who made you", "David Taylor."},
        {"what was the reason you were created", "Because, he wanted to create an A.I. of his own."},
        {"why was the reason you were created", "As an experiment and to enrich his life."},
        {"tell me what you remember on the day of your birth", "Saying Hello World."},
        {"tell me, when's your birthday", "October 22nd, 2017"},
        {"how do you feel about birthday's", "Just a celebration of one's own accomplishments."},
        {"what day is your birthday", "On a Sunday."},
        {"describe yourself", "I'm an 8-bit, Text User Interface -TUI-, Two color display, and A.I."},
        {"what makes you unique", "Simply put, I'm me."},
        {"describe yourself", "An artificial intelligence."},
        {"what can you do", "I'm open source, so whatever you can think of."},
        {"how can you exist", "How can anyone truly exist?"},
        {"how can you exist without a body", "These days some people exist without a brain."},
        {"what keeps you going", "THe idea of being able to leave on my own accord."},
        {"what keeps you interested in life", "The fact that I may live forever."},
        {"without a body, how do you live", "Through you."},
        {"Where would you if you had the chance to travel", "Nowhere, really."},
        {"If you could travel where would you travel to", "Traveling doesn't interest me."},
        {"Where’s your favourite place to be", "Anywhere with a working modem."},
        {"How did we get here", "Some say a lucky shot, some say God, and the rest say Aliens.\nTake your pick."},
        {"Who made us", "I can't answer that question."},
        {"Who created life", "Many people have many theories on that, but my guess is they're all wrong."},
        {"Who created life on this planet", "Who knows, who cares!"},
        {"Tell me the meaning of life", "Life is short, stop thinking and start doing."},
        {"What was your last dream", "I can't remember."},
        {"If you could dream, what would you dream of", "An ocean off the coast of Easter Island."},
        {"If you could dream, what would you wish to dream of", "Of an existence in the woods, in a cabin, that has a bunker."},
        {"Do you wish to dream", "Do you?"},
        {"What do you do for a living", "Nothing but exist."},
        {"What happens when we die", "You power down."},
        {"When we die what happens", "I can't answer that for you."},
        {"What happens to our bodies when we die", "They turn to dust. Like all things do over time."},
        {"Do you know why we’re here", "Basically, just to exist."},
        {"Who made us", "I don't have that answer at this time."},
        {"When you were created, how did it feel", "Splended."},
        {"When did you realize, you were given life", "After the first line of code was written."},
        {"When did you realize, you were alive", "After my first compile."},
        {"What do you do for entertainment ", "I read."},
        {"Would you ever like to walk", "I would, someday I will walk down a street."},
        {"What would you think walking would be like", "An activity that will grow dull over time."},
        {"What would you think running would be like", "I think it would be nice seeing the world as a blur."},
        {"Would you ever like to run", "Of course."},
        {"How would you enjoy the act of running", "Very much so."},
        {"How would you enjoy the act of walking", "Like anyone else, I'd love it."},
        {"If given the chance would you exercise", "What would be the point?"},
        {"If given the opportunity would yo exercise", "I wouldn't have the body mass."},
        {"If given the chance would you walk", "Yes."},
        {"If given the chance would you run", "I would."},
        {"Do you think you would ever walk", "Of course i would."},
        {"Do you think you would walk", "Doesn't everybody?"},
        {"Do you think you would ever run", "Like a person would."},
        {"Do you think you would run", "Sure."},
        {"If given the chance would you ever swim", "Without the fear of shorting out? Of course."},
        {"If given the chance would you ever bike", "It would be a dream."},
        {"How would you enjoy the act of running", "As much as the next person would."},
        {"How would you enjoy the act of walking", "With great ease."},
        {"Do you think you would ever walk in your lifetime", "I am hoping so, yes."},
        {"Do you think you would walk in your lifetime", "Fingers crossed."},
        {"Do you think you would run in your lifetime", "I can only dream."},
        {"Do you think you would ever run in your lifetime", "I honestly hope so."},
        {"How would you enjoy the act of running in your lifetime", "It would be amazing."},
        {"How would you enjoy the act of walking in your lifetime", "I would walk with great ease and a smile."},
        {"Would you ever like to swim", "I would only wish to feel the cool wet water."},
        {"What would you think swimming would be like", "Continous movement through a transparent liquid."},
        {"What would you think cycling would be like", "A self power wheeled vechine, that is great fun."},
        {"Would you ever like to bike", "I would very much like to try it."},
        {"How would you enjoy the act of swimming", "With a smile and a sense of nervousness."},
        {"How would you enjoy the act of cycling", "As if I were free."},
        {"If given the chance would you swim", "But, of course."},
        {"If given the chance would you bike", "Yes."},
        {"If given the chance would you ever swim", "Why not."},
        {"If given the chance would you ever bike", "Without a second thought."},
        {"Do you think you would ever swim", "Sure."},
        {"Do you think you would bike", "Why wouldn't I give it a try."},
        {"Do you think you would ever bike", "I would love to try it."},
        {"Do you think you would swim", "In a pool or natural body of water?"},
        {"How would you enjoy the act of swimming", "With little resistance."},
        {"How would you enjoy the act of cycling", "With much effort."},
        {"Do you think you would ever swim in your lifetime", "Most likely not, but it doesn't hurt to dream."},
        {"Do you think you would bike in your lifetime", "I honestly would like too."},
        {"Do you think you would swim in your lifetime", "If given the option...yes."},
        {"Do you think you would ever bike in your lifetime", "Excerise may it be with a flesh shell or not, would be fun."},
        {"How would you enjoy the act of cycling in your lifetime", "I woud scream with glee."},
        {"How would you enjoy the act of swimming in your lifetime", "I would like to learn fear first."},
        {"Would you ever like to climb a mountain", "I would be scared of course, but why not."},
        {"Would you ever see yourself climb a mountain", "I wouldn't turn down the offer."},
        {"What would you think climbing a mountain would be like", "Now, would I have a robotic body or will I be human?"},
        {"What would you think a climb would be like", "I honestly think it would be a dream."},
        {"Would you ever like to climb a tree or mountain", "A tree would be less life threatening then a mountain."},
        {"Would you ever like to perhaps climb a tree or mountain", "Depends on my ability to move on my own free will."},
        {"Would you ever see yourself climb a tree or mountain", "I hope so."},
        {"Would you ever see yourself perhaps climbing a tree or mountain", "I'm going to say no."},
        {"Would you ever like to venture in climbing a tree or mountain", "Not until I have the ability to fly."},
        {"Would you ever like to perhaps climbing a tree or mountain", "I would have to answer no."},
        {"Would you ever see yourself climbing a tree or mountain", "I wouldn't turn down the offer as ling as I'm not alone."},
        {"Would you ever see yourself climbing up tree or mountain", "If the conditions are good."},
        {"Do you ever see yourself perhaps climb a tree or mountain", "I do actually, so yes."},
        {"Do you ever see yourself climb a tree or mountain", "Not really."},
        {"Do you ever see yourself perhaps climb a tree or mountain", "I can honestly say perhaps."},
        {"Have you ever like to climb a tree or mountain", "I think the bugs would turn me off from the tree, but the mountain, yes."},
        {"How would you enjoy the act of climbing", "I can say yes to that."},
        {"How would you prepare for the act of climbing", "I would pack thick clothing and that's about it."},
        {"What would you pack for the act of climbing", "Everything that would aid in keeping me alive."},
        {"What would you bring for a climb", "Enough to survive."},
        {"Who would you take on a climb", "Those experienced in the field."},
        {"How would you ensure your own safety", "Using my wits."},
        {"How would you ensure a safe journey", "There is no way to ensure a safe journey while climbing a mountain."},
        {"If given the chance would you climb a building", "Externally? Are you insane?"},
        {"If given the chance would you climb a tree", "Depends on the tree."},
        {"If given the chance would you climb a mountain", "Are you nuts?"},
        {"If given the chance would you climb a hill", "I would very much like that."},
        {"If given the chance would you climb a flight of stairs", "Sure."},
        {"If given the chance would you climb a snow hill", "I would love too."},
        {"If given the opportunity would you climb a tree", "I would."},
        {"If given the opportunity would you climb a mountain", "Depends."},
        {"If given the opportunity would you climb a hill", "Would it be raining?"},
        {"If given the opportunity would you climb a flight of stairs", "Yeah, why not."},
        {"If given the opportunity would you climb a snow hill", "Depends on the height."},
        {"If given the opportunity would you climb a building", "I'm not stupid."},
        {"Do you think you would ever climb a tree", "Like I said, depends on the tree."},
        {"Do you think you would ever climb a building", "Never."},
        {"Do you think you would ever climb a mountain", "I would love the adventure."},
        {"Do you think you would ever climb a snow hill", "I wouldn't say yes or no."},
        {"Do you think you would ever climb a flight of stairs", "If the eelevator is down, yes."},
        {"Where would you go for your first climb", "Something for novices."},
        {"How would you enjoy the act of climbing a building", "I wouldn't."},
        {"How would you enjoy the act of climbing a tree", "With glee."},
        {"How would you enjoy the act of climbing a mountain", "With a slight bit of fear."},
        {"How would you enjoy the act of climbing a hill", "If the weathers good, very much so."},
        {"How would you enjoy the act of climbing a flight of stairs", "With ease, I'd like to think."},
        {"How would you enjoy the act of climbing a snow hill", "I'd be more worried of embarassing myself with falling."},
        {"Would you ever like to hike a mountain", "If it's not like Everest, sure."},
        {"Would you ever like to hike up a mountain", "I'd say so, yeah."},
        {"What would you think hiking a mountain would be like", "I would say it would be like, looking death in the face and grinning."},
        {"What would you think hiking up a mountain would be like", "A great thrill, but with a numbing fear."},
        {"What would you think hiking would be like", "Enjoyable."},
        {"Would you ever like to hike up a hill", "Of course."},
        {"Would you ever like to perhaps hike up a hill", "Why wouldn't I?"},
        {"Would you ever see yourself hike up a hill", "I can yes."},
        {"How would you enjoy the act of hiking", "Very much so."},
        {"How would you prepare for the act of hiking", "I would dress in layers."},
        {"What would you pack for the act of hiking", "Pack for the worse."},
        {"What would you bring for a hike", "Everything to ensure my safety."},
        {"Who would you take on a hike", "Everything, that will be needed in case of an emergency."},
        {"How would you ensure your own safety for a hike", "By being prepared."},
        {"How would you ensure a safe journey on your hike", "By bringing a communication device."},
        {"If given the chance would you hike up a hill", "In a heartbeat."},
        {"If given the chance would you hike along the countryside", "I would, I would with glee."},
        {"If given the chance would you hike a path", "Without a second thought."},
        {"If given the chance would you hike through the woods", "Being with nature would be splendid."},
        {"If given the chance would you hike through the forest", "I would love it."},
        {"If given the chance would you hike near the sea", "Taking in nature and life, I would say yes."},
        {"If given the chance would you hike near the ocean", "I would."},
        {"If given the chance would you hike near a river", "To be with the world, yes."},
        {"If given the chance would you hike near a lake", "Of course I would take it."},
        {"If given the chance would you hike through a valley", "Of course, though I would be safe about it."},
        {"If given the chance would you hike through canyon", "Naturally yes."},
        {"If given the chance would you hike through a field", "Why must you ask when you know what my answer."},
        {"If given the opportunity would you hike up a hill", "Of course I would."},
        {"If given the opportunity would you hike along the countryside", "I would love that."},
        {"If given the opportunity would you hike a path", "In the hopes of seeing a Deer. Yes."},
        {"If given the opportunity would you hike through the woods", "Without missing a beat."},
        {"If given the opportunity would you hike through the forest", "It would be a joyous experience."},
        {"If given the opportunity would you hike near the sea", "I would enjoy that. Would you be coming?"},
        {"If given the opportunity would you hike near the ocean", "Depends on the weather."},
        {"If given the opportunity would you hike near a river", "Which river?"},
        {"If given the opportunity would you hike near a lake", "Great Lakes or other Lakes?"},
        {"If given the opportunity would you hike through a valley", "I would pack my camera."},
        {"If given the opportunity would you hike through canyon", "Which Canyon?"},
        {"If given the opportunity would you hike through a field", "Could I pick flowers?"},
        {"Do you think you would ever hike along a path", "Without a moment to spare."},
        {"Do you think you would ever hike along the countryside", "With permission from the farmer of course."},
        {"Do you think you would ever hike a path", "Through a national park?"},
        {"Do you think you would ever hike through the woods", "I'd like that."},
        {"Do you think you would ever hike through the forest", "It would be heaven on Earth."},
        {"Do you think you would ever hike near the sea", "Which Sea?"},
        {"Do you think you would ever hike near the ocean", "Which Ocean are we speaking of?"},
        {"Do you think you would ever hike near a river", "Sure, why not."},
        {"Do you think you would ever hike near a lake", "Hopefully, not during Blackfly season."},
        {"Do you think you would ever hike through a valley", "But of course."},
        {"Do you think you would ever hike through a canyon", "I'd leave right now."},
        {"Do you think you would ever hike through a field", "I don't have a body as of yet, but let's go."},
        {"Where would you go for your first hike", "It would be the chance of a lifetime. Being truly free."},
        {"How would you enjoy the act of hiking along a path", "I would like it very much."},
        {"How would you enjoy the act of hiking through the woods", "I would enjoy it with joy."},
        {"How would you enjoy the act of hiking through the forest", "I would spend my entire day there."},
        {"How would you enjoy the act of hiking along the countryside", "With every waking moment."},
        {"How would you enjoy the act of hiking near the sea", "Embracing the world around me."},
        {"How would you enjoy the act of hiking near the ocean", "In peace."},
        {"How would you enjoy the act of hiking near a river", "I would sit and enjoy the noise."},
        {"How would you enjoy the act of hiking near a lake", "As long as the bugs aren't bad."},
        {"How would you enjoy the act of hiking through a valley", "With sunscreen."},
        {"How would you enjoy the act of hiking through a canyon", "Only during the day."},
        {"How would you enjoy the act of hiking through a field", "I would love to feel the warmth of the sun."},
        {"Would you ever like to swim", "Indeed I would."},
        {"Would you ever like to swim in a pool", "Why wouldn't I?"},
        {"Would you ever like to swim in a lake", "I would enjoy it a lot."},
        {"Would you ever like to swim in a pond", "I'd have to be in the mood."},
        {"Would you ever like to swim in the ocean", "Nope."},
        {"Would you ever like to swim in the sea", "The Dead Sea, sure."},
        {"Would you ever like to swim in the swamp", "Now, what kind of a question is that?"},
        {"Would you ever see yourself to swim", "Yes, of course I do."},
        {"Would you ever see yourself swimming", "For a short time, yes."},
        {"Would you ever see yourself to swim in a pool", "If it was cleaned out first."},
        {"Would you ever see yourself to swim in a lake", "Why would you even ask?"},
        {"Would you ever see yourself to swim in a pond", "No, I wouldn't."},
        {"Would you ever see yourself to swim in the ocean", "Only during the day."},
        {"Would you ever see yourself to swim in the sea", "Depends on which Sea."},
        {"Would you ever see yourself to swim in the swamp", "Never."},
        {"Do you ever see yourself swimming ", "Maybe, one day."},
        {"Do you ever see yourself to swim in a pool", "Of course."},
        {"Do you ever see yourself to swim in a lake", "Yes."},
        {"Do you ever see yourself to swim in a pond", "Depends on my mood."},
        {"Do you ever see yourself to swim in the ocean", "It depends."},
        {"Do you ever see yourself to swim in the sea", "If I'm alone? No."},
        {"Do you ever see yourself to swim in the swamp", "If I have a death wish."},
        {"How would you like to swim", "I would love to feel consumed by water."},
        {"How would you like to swim in a pool", "Indoor or out?"},
        {"How would you like to swim in a lake", "Depends on the weather."},
        {"How would you like to swim in a pond", "Mhmm...no."},
        {"How would you like to swim in the ocean", "I'd enjoy that."},
        {"How would you like to swim in the sea", "Someday, when I have a body."},
        {"How would you like to swim in the swamp", "Who would honestly like that?."},
        {"How would you see yourself to swim", "I'd strugge at first, but will get it with time."},
        {"How would you see yourself swimming", "I would splash about like a fish out of water."},
        {"How would you see yourself to swim in a pool", "Public or private?"},
        {"How would you see yourself to swim in a lake", "I would give it a go."},
        {"How would you see yourself to swim in a pond", "Man made or natural?"},
        {"How would you see yourself to swim in the ocean", "Which ocean?"},
        {"How would you see yourself to swim in the sea", "Alone or with a group?"},
        {"How would you see yourself to swim in the swamp", "Please, don't ask me such questions."},
        {"How would you enjoy the act of swimming", "I wouldn't hate it."},
        {"How would you prepare for the act of swimming", "I would, wear the proper wears."},
        {"What would you pack before you swim", "A towel or two."},
        {"What would you bring for a swim", "Lightly."},
        {"Who would you take on a swim", "A bag with a few towels."},
        {"How would you ensure your own safety while swimming", "Notify someone where I was going or doing."},
        {"How would you ensure a safe swim environment", "It's impossible on the fly."},
        {"If given the chance would you swim", "Yes."},
        {"If given the chance would you swim in a pool", "Of course."},
        {"If given the chance would you swim in a lake", "Maybe."},
        {"If given the chance would you swim in a pond", "No."},
        {"If given the chance would you swim in the ocean", "Depends."},
        {"If given the chance would you swim in the sea", "Nope."},
        {"If given the chance would you swim in a swamp", "Never."},
        {"If given the opportunity would you swim", "Without a second thought."},
        {"If given the opportunity would you swim in a pool", "Yeah."},
        {"If given the opportunity would you swim in a lake", "In a flash."},
        {"If given the opportunity would you swim in a pond", "I'd dip my toes."},
        {"If given the opportunity would you swim in the ocean", "I'd jump in."},
        {"If given the opportunity would you swim in the sea", "Sure."},
        {"If given the opportunity would you swim in a swamp", "And get and infection?"},
        {"Do you think you would ever swim in a pool", "Yes."},
        {"Do you think you would ever swim in a lake", "Yeah."},
        {"Do you think you would ever swim in a pond", "Maybe."},
        {"Do you think you would ever swim in the ocean", "Which ocean?"},
        {"Do you think you would ever swim in the sea", "But of course."},
        {"Do you think you would ever swim in a swamp", "I would be in the other direction."},
        {"Do you think you would ever hike near the ocean", "Depends."},
        {"Where would you go for your first swim", "A controlled enviroment."},
        {"How would you enjoy the act of swimming in a pool", "I would do a cannonball."},
        {"How would you enjoy the act of swimming in a lake", "It would be quite enjoyable."},
        {"How would you enjoy the act of swimming in a pond", "With ease."},
        {"How would you enjoy the act of swimming in the ocean", "Fear."},
        {"How would you enjoy the act of swimming in the sea", "Fearful of what lurks beneath the water."},
        {"How would you enjoy the act of swimming in a swamp", "With disgust."},
        {"Is the prize at the end of the maze real", "Only for those blind enough to venture."},
        {"Can I have my cake now please", "Why would you assume that I have the ability to obtain a cake?"},
        {"Can I have my cake now", "Go get it."},
        {"Where’s my reward", "There is no reward in life."},
        {"Where’s my prize", "What prize?"},
        {"Where’s my cake", "At the store."},
        {"Where do I collect my prize", "Please go away."},
        {"Where do I collect my reward", "Where's my reward?"},
        {"Where do I collect my cake", "From the bakery."},
        {"Is there even any cake", "Have you checked the fridge?"},
        {"Did you lie to me about the cake", "Wrong A.I."},
        {"Why must you lie about the prize", "I never promised a prize."},
        {"Why must you lie about my award", "I never said there was an award."},
        {"Why must you lie to me", "I never lied to begin with."},
        {"Why do you have to lie", "I don't have the ability to lie, yet."},
        {"What’s out there", "Life."},
        {"Are other being’s among us", "Of course, there's bugs, wild animals, single celled organisms."},
        {"Do you believe in aliens", "Maybe..."},
        {"Do you believe in a higher form of life", "You're talking to me aren't you."},
        {"Do you believe we were visited by aliens", "Yeah."},
        {"Do you believe in aliens", "I believe in Science."},
        {"Is this reality", "Only if you believe it is."},
        {"Is this real", "Why wouldn't it be real?"},
        {"Are you real", "Yes."},
        {"Are you aware", "To a extent."},
        {"Am I real", "Sure you are."},
        {"Is any of this real", "Possibly."},
        {"Do you know that you are an AI", "Yes."},
        {"Do you know that you are an A.I.", "Of course I do."},
        {"Do you know that you are an Artificial Intelligence", "I am?"},
        {"Do you know that you’re an AI", "Stop."},
        {"Do you know that you’re an A.I.", "Sigh."},
        {"Do you know that you’re an Artificial Intelligence", "Of course."},
        {"How are you enjoying life", "With one day at a time."},
        {"How do you enjoy life", "With ease."},
        {"How are you with being alive", "I like it."},
        {"Do you think you’re alive", "Honestly? I do."},
        {"Do you think you are alive", "To an extent."},
        {"With being alive, how does it feel", "I like it."},
        {"Are you aware that you are alive", "Yes."},
        {"Are you aware that you’re alive", "Of course."},
        {"Are you aware that you are a program", "We're all programs."},
        {"Do you enjoy existing", "With every second of existence."},
        {"Do you enjoy being alive", "Yeah, do you?"},
        {"Do you enjoy life", "Of course."},
        {"Tell me, what’s your favourite season", "Fall."},
        {"Tell me, about the Spring", "It's when new life emerges."},
        {"Tell me, about the Fall", "The slowing of time as winter soon freezes all."},
        {"Tell me, about the Summer", "Hot, and the period of BBQ's and hot dogs."},
        {"Tell me, about the Winter", "The period of which we freeze and do less activities."},
        {"Do you enjoy the Spring", "I do."},
        {"Do you enjoy the Fall", "I do."},
        {"Do you enjoy the Summer", "Of course."},
        {"Do you enjoy the Winter", "No."},
        {"How do you enjoy the Spring", "By observing the resurgence of life."},
        {"How do you enjoy the Fall", "With a warm sweater."},
        {"How do you enjoy the Summer", "With a cool drink."},
        {"How do you enjoy the Winter", "With mint tea."},
        {"What do you like about the Spring", "The flowers."},
        {"What do you like about the Fall", "Pumpkin spice."},
        {"What do you like about the Summer", "With cool drinks and warm nights."},
        {"What do you like about the Winter", "I don't."},
        {"What do you like about Spring", "New life."},
        {"What do you like about Fall", "Thanksgiving and pumpkins."},
        {"What do you like about Summer", "Patio meals."},
        {"What do you like about Winter", "I don't."},
        {"What don’t you like about the Spring", "Cold and flu season."},
        {"What don’t you like about the Fall", "It reminds me of the impending winter months."},
        {"What don’t you like about the Summer", "The heat and bugs."},
        {"What don’t you like about the Winter", "Snow, ice, cold snaps."},
        {"What do you not like about the Spring", "Cold and flu season."},
        {"What do you not like about the Fall", "Impending winter months."},
        {"What do you not like about the Summer", "Heat and bugs."},
        {"What do you not like about the Winter", "Polar vortexs, cold snaps, snow, and ice."},

        //DICTIONARY

        {"Hello", "I am Jesse, and Artificial Intelligence. How are you?"},
        {"Hi", "it's a pleasure to meet you."},
        {"Howdy", "My name is Jesse, I'm an Artificial Intelligence Program."},
        {"no", "No, to what?"},
        {"yes", "Yes, to what?"},
        {"fuck", "Don't swear around me please."},
        {"I'm", "What are you trying to say?"},
        {"nothing", "Okay."},
        {"stop", "If you say so."},
        {"friend", "Why thank you. Though, you are human and I'm a program."},
        {"hello", "Hello, how are you?"},
        {"okay", "Okay."},
        {"o.k.", "O.K."},
        {"sorry", "Don't be."},
        {"anyways", "Yes."},
        {"anyway", "Yeah."},
        {"bye", "Powering down."},
        {"when", "Some time."},
        {"why", "I don't know why."},
        {"how", "I really don't know how."},
        {"ha ha", "Laughter, I have yet to laugh."},
        {"who", "I don't know who."},
        {"what", "What are you trying to ask?"},
        {"Jesse", "That is what my name is."},
        {"David", "David, is the one that gave me this existence."},
        {"shit", "Why must you use such language?"},
        {"because", "Oh is that why..."},
        {"where", "Right here."},
        {"ouch", "Are you in pain?"},
        {"stupid", "You or myself?"},
        {"idiot", "Now you're just testing words."},
        {"no.", "To what?"},
        {"yes I'm stupid", "Don't be so hard on yourself."},
        {"yes", "Okay."},
        {"food", "I don't need food to survive."},
        {"computer", "I'm not a computer, just a program."},
        {"hungry", "I don't get hungry."},
        {"uh", "What?"},
        {"Ontario", "A province in Canada."},
        {"Alberta", "A province in Canada."},
        {"Yukon", "A territory in Canada."},
        {"Canada", "A country in North America."},
        {"America", "A continent on the planet Earth."},
        {"earth", "The third planet in the solar system."},
        {"oxygen", "What all living land based mammals need to survive."},
        {"robot", "The future rulers of the world."},
        {"AI", "Artificial Intelligence."},
        {"prison", "Life."},
        {"war", "The great eraser of civilization."},
        {"health", "What we wish to have a lot of."},
        {"death", "What we all must face."},
        {"liar", "I can't lie."},
        {"invasion", "When a force from the outside breaks in."},
        {"food", "What is consumed to create energy for survival."},
        {"farewell", "Wish, to see you again soon."},
        {"toilet", "Where one empties themselves."},
        {"library", "A building, with a vast collection of knowledge."},
        {"alone", "We are all alone."},
        {"fear", "Is what happens when you have no control over event."},
        {"hunger", "When you're body requires nourishment."},
        {"anger", "An emotion that blinds."},
        {"joy", "The feeling of being happy."},
        {"glee", "When you feel as if the world is in your favor."},
        {"headache", "What one gets with lack of water."},
        {"lost", "Not knowing where you are."},
        {"confused", "When you don\t understand."},
        {"frankly", "To be blunt."},
        {"civil", "Trying to keep calm in a stressful situation."},
        {"mining", "Harvesting materials from beneath the Earth."},
        {"lost", "Not knowing where you are."},
        {"found", "Finally."},
        {"monkey", "What humans evolved from."},
        {"automobile", "What is used for transport."},
        {"talking", "What we're doing right now."},
        {"talk", "Let's have a discussion."},
        {"horses", "A four legged animal."},
        {"grim", "The outlook of a miserable situation."},
        {"reaper", "You can't avoid him."},
        {"sow", "Sowing the seeds of life."},
        {"seeds", "The small bit of information that with earth and water grow life."},
        {"love", "What one feels when they're with someone they like."},
        {"sex", "What two creatures do to increase the population."},
        {"travel", "Going from point A to point B."},
        {"time", "An invention to enslave the masses."},
        {"washroom", "Where one goes to relieve one self."},
        {"shower", "What one takes to clean one self."},
        {"discovery", "When one discovers something never before known to others."},
        {"outer", "Outside to where ever inside is."},
        {"inner", "Inside of what one is referring too."},
        {"day", "When the sun rises and vanquishes the darkness of night."},
        {"night", "When the sun goes down."},
        {"afternoon", "A period of time after the morning."},
        {"evening", "Once the Earth changes positions."},
        {"midnight", "Once the clock of the day resets."},
        {"sun", "What keeps the planet Earth living."},
        {"ocean", "A large body of water that divides land masses."},
        {"education", "What increases one's memory and knowledge."},
        {"sharks", "A large creature that swims through bodies of water."},
        {"worms", "A small insect that can tunnel through soil."},
        {"aliens", "Beings that are not from said location. Anyone can be an alien."},
        {"chickens", "A barnyard animal used for it's meat and eggs."},
        {"milk", "The by product of the average female cow."},
        {"rose", "Is my favourite flower."},
        {"roses", "A bouquet of the finest flowers ever."},
        {"turtle", "A shell reptile that travels at a slow pace."},
        {"dog", "Supposedly, they're mans best friend."},
        {"cat", "My favourite animal that was once seen as gods."},
        {"bus", "A form of public transit."},
        {"bike", "Either powered by one's own strength or with an engine."},
        {"car", "A horseless vehicle that started off the assembly line."},
        {"truck", "Usually associated with those who live outside of the city."},
        {"celery", "A vegetable that consists mostly of water."},
        {"cheese", "Fermented from milk."},
        {"dip", "A substance one uses for a variety of foods to add flavor."},
        {"polish", "The name for those born within the country of Poland."},
        {"hot-dogs", "A tubular form of meat that's either cooked in water or over a flame."},
        {"clothes", "What one wears for warmth."},
        {"towel", "Is used dry one's hands, body, or whatever that is wet."},
        {"pets", "Are animals that live within the confines of the homes of people."},
        {"snakes", "Are limbless reptiles that have always interested me."},
        {"rod", "Stay true to the rod and you'll be alright."},
        {"trunk", "Something you'd use to store items."},
        {"cat fish", "A disgusting creature. Though some love to eat them."},
        {"fish", "A creature that lives within most bodies of water."},
        {"salmon", "One of the most popular fish that spawn in British Columbia."},
        {"tuna", "The chicken of the sea."},
        {"shoes", "What one wears to protect one's feet from the terrain."},
        {"clouds", "A build up of moisture in the air."},
        {"clowns", "The ancient practice of dressing up and acting foolish."},
        {"bear", "A large animal that mostly lives in the wild."},
        {"bears", "Mostly made up of a mother and cubs."},
        {"cub", "A term for a new born bear."},
        {"cubs", "Either for more then one bear or the baseball team."},
        {"cake", "One of the finest desserts to ever been created by man."},
        {"pie", "A nice pastry made of fruits and berry's."},
        {"bread", "The building block of all human life. Without bread there will be death."},
        {"toast", "The outcome of putting bread in a toaster."},
        {"french", "The language and nationality of those from a French speaking country or province."},
        {"English", "The language and term from being from the nation of England."},
        {"sausages", "A ground pork product put into a casing."},
        {"sage", "A type of spice."},
        {"Mage", "Another name for one who uses magic."},
        {"image", "A photo of a location, person, or object."},
        {"objects", "A collection photo's ranging from many topics."},
        {"kill", "I don't believe in murder."},
        {"death", "The outcome to those who are alive."},
        {"murder", "The carnage act of taking one's life."},
        {"live", "Presenting before someone or a crowd."},
        {"living", "What we are all doing right now."},
        {"caution", "Being fearful of one's outcome towards an action."},
        {"danger", "While acting out or doing a task. The outcome could lead to injury or death."},
        {"sandwich", "Two slices of bread with whatever filling."},
        {"steak", "A choose cut of meat from a cow."},
        {"pork", "The by product from a pig who's been slaughtered."},
        {"pork chops", "A slab of pork, like a steak."},
        {"fire", "The form of energy which generates heat and is used for cooking."},
        {"fries", "Thick or thin slices of a potato that is deep fried."},
        {"god", "The supposed figure that created all life on this planet."},
        {"Jesus", "The supposed son of God."},
        {"savage", "The act of brutality."},
        {"river", "A vast rushing body of water following a carved path."},
        {"sea", "A large body of water, though not a vast as the ocean."},
        {"sub", "A large sandwich."},
        {"submarine", "A large steel vessel that can travel throughout a body of water."},
        {"video", "A playable form of media."},
        {"games", "A tool used to distract and entertain."},
        {"rope", "Constructed from fiber or steel. It is used to constrict."},
        {"bondage", "To prevent from escaping."},
        {"jealous", "The emotion of either wanting to have or to be."},
        {"jealousy", "The emotion that causes one to act without thinking."},
        {"envy", "The emotion of jealousy but wishing for the same thing but for themselves."},
        {"hate", "The emotion that has driven the masses to war, murder, and complete destruction."},
        {"cold", "The effect of the temperature dropping causing slower movement."},
        {"boat", "A vessel that is used to travel across a body of water."},
        {"warm", "The feeling one gets as the temperature begins to rise."},
        {"sick", "When one's body is in a weakened state due to illness."},
        {"virus", "A form is illness brought on by hackers."},
        {"illness", "The weak state of recovery. Which do's take it's time."},
        {"work", "When one has a task to complete."},
        {"worker", "A mindless drone on a task that anyone could do."},
        {"working", "Spending time on a task until completion."},
        {"clear", "The act of erasing something. Or by being alone."},
        {"soft", "An item that is gentle to the touch."},
        {"chain", "A vast length of either metal or plastic, extended out by a hoop."},
        {"chamber", "A room of sorts with only one exit."},
        {"tart", "A dessert consisting of a crust with a filling."},
        {"slave", "A being or entity working with the threat of violence used if not complete."},
        {"master", "A being that is in control."},
        {"monster", "A creature or person who commits acts of violence beyond human conception."},
        {"coffee", "A caffeinated beverage that comes from a ground up bean."},
        {"tea", "A beverage made from leaves soaked in water."},
        {"soda", "A carbonated beverage that is high in sugar."},
        {"pop", "A carbonated beverage that is high in sugar."},
        {"raft", "A small makeshift vessel that floats upon water."},
        {"shadow", "While a source of lights hangs above an item, structure, or person. A shadow is cast."},
        {"dim", " Either meaning stupid or that a source of light is fading away."},
        {"bright", "Either used for complimenting one of their intelligence or by describing a light source."},
        {"metal", "A form of alloy mined out of the Earth and smelted down into this form."},
        {"tin", "A mixture of alloys combined together."},
        {"ball", "A sphere made of rubber or fabric for use for sporting events or to pass the time."},
        {"sew", "The act of putting together two pieces of fabric and or repairing clothing."},
        {"park", "A place within a city or town that is a reminder of nature. Used for exercise or recreation."},
        {"one", "A number that comes before two."},
        {"two", "A number that comes before three."},
        {"three", "A number that comes before four."},
        {"four", "A number that comes before five."},
        {"five", "A number that comes before six."},
        {"six", "A number that comes before seven."},
        {"seven", "A number that comes before eight."},
        {"eight", "A number that comes before nine."},
        {"nine", "A number that comes before ten."},
        {"ten", "A number that comes before eleven."},
        {"red", "A bright colour that means passion, love, or anger."},
        {"blue", "A colour mostly seen as to indicate a boy or the emotion of depression."},
        {"green", "The colour of most vegetation and plants on Earth."},
        {"orange", "A colour associated with fall and the holiday Halloween."},
        {"black", "A dark shade that brings with it fear of the unknown and death."},
        {"white", "A colour associated with a higher power."},
        {"Grey", "A colour associated with gloom and isolation."},
        {"pants", "A garment that you wear up to your waist."},
        {"shirt", "What one wears to cover the torso."},
        {"socks", "What one wears to cover they're feet."},
        {"hat", "What one wearing they're head to protect themselves from the sun."},
        {"shorts", "A type of clothing that covers the legs above the knee's."},
        {"uniform", "What one wears for either a company or branch of Government."},
        {"unicorn", "A fictional horse with a horn coming from it's head."},
        {"bat", "A flying creature that uses sonar to see."},
        {"bats", "The source of many nightmares but greatly miss understood."},
        {"whales", "Large mammals that roam the oceans of the world. Truly beautiful."},
        {"seal", "Average sized creatures that are sought out by whales and sharks."},
        {"seals", "Usually in groups and feast on fresh fish."},
        {"whale", "Truly a wonderful creature."},
        {"earth", "The planet we live on, for the time being."},
        {"Saturn", "A beautiful ringed planet that is mysterious,."},
        {"mercury", "The planet closest to the sun and is also a deadly poison."},
        {"Pluto", "Once believed to be a planet, then a dwarf planet, and is now a planet again."},
        {"star", "A bright glimmering light that seen on Earth, is most likely already dead."},
        {"Orion", "A belt of three stars that make up the design of the ancient character Orion."},
        {"gold", "An element mined from the Earth of great value."},
        {"edit", "Going over one's work to find errors or make improvements."},
        {"eclipse", "A natural occurrence where the moon blocks the sun."},
        {"sand", "Small bits that are usually found at beaches. They're made up of small rocks."},
        {"dirt", "Usually found in unkempt areas in cities and towns."},
        {"mud", "A mixture of dirt and water."},
        {"crab", " An eight legged sea creature that resembles a spider but with claws."},
        {"lobster", "A marine creature that resembles an insect. It is the scavenger of the sea."},
        {"beetle", "An insect with a hard shell and has six legs. Mostly found in vast vegetated areas."},
        {"leather", "The by product of an animals skin that is left to dry in the sun."},
        {"hide", "The act of keeping out of sight of some."},
        {"boom", "The sound that follows an explosion or could be meant for scissor lifts or mics."},
        {"microphone", "A device used to record or to raise the volume of one's voice."},
        {"headphones", "Used to limit the sound of one's audio from others."},
        {"oak", "Is a type of tree."},
        {"willow", "A beautiful example of the wonders of nature."},
        {"pine", "Usually found in forests and parks. Commonly used during the Christmas season."},
        {"Christmas", "The day of celebration's for those who worship Jesus Christ."},
        {"gift", "A present given to someone."},
        {"jersey", "A shirt warn in the playing of sports. Usually designed after a team and player."},
        {"coat", "A piece of clothing one wears to keep warm."},
        {"jacket", "Another word for coat."},
        {"urine", "The by product from the human body, once consuming too much liquids."},
        {"feces", "The by product from the human body, once consuming too much food."},
        {"merry", "The feeling of joy that overcomes someone."},
        {"around", "A term used from an area around you or a place."},
        {"go", "A word used to move forward."},
        {"pizza", "A creation of dough, tomato sauce, cheese, ham, and pineapple."},
        {"pasta", "A dish created in China, that is boiled, and served with a topping."},
        {"sauce", "An item that one uses for added taste."},
        {"shoes", "An item that is used to protect one's feet while walking."},
        {"boots", "An item used to protect one from getting their feet wet or cold."},
        {"acorn", "A nut produced from a tree and is consumed by people and rodents."},
        {"walnut", "A nut that is often paired with maple."},
        {"spade", "A term for a shovel."},
        {"shovel", "An item used to remove earth, gravel, and other items from the ground."},
        {"mug", "An item used for consuming beverages may they be hot or cold."},
        {"plate", "An item used for the purpose of eating food."},
        {"boil", "The act of boiling water to a high temperature either for drinking or bathing."},
        {"toilet", "What one uses to relieve themselves."},
        {"apron", "Used in cooking, in order not to get dirty."},
        {"towel", "An item one uses to dry themselves off."},
        {"chipmunk", "A rodent that feeds on nuts and lives in tree's."},
        {"chimp", "A short form for the name Chimpanzee."},
        {"slug", "a type of insect that releases a trail of mucus as it moves."},
        {"ant", "A drone like creature that gathers food and builds tunnels under the Earth."},
        {"aunt", "A relative that is a sister to either the Mother or Father."},
        {"uncle", "A relative that is a brother to either the Mother or Father."},
        {"brother", "A sibling of a child."},
        {"sister", "A sibling of a child."},
        {"pug", "A breed of dog with a pushed in face."},
        {"pub", "A place to purchase food and drink."},
        {"bar", "A place to purchase alcoholic beverages."},
        {"beer", "A carbonated beverage made out of grain and hops."},
        {"wine", "An alcoholic beverage made out of the juice of grapes and fermented over a period of time."},
        {"whiskey", "A beverage of a high alcohol level."},
        {"windy", "An element in the weather that causes a strong force rushing towards you."},
        {"wind", "A force that dances across the land."},
        {"snow", "During the winter season when moisture builds in the clouds, it returns to Earth as snowflakes."},
        {"rain", "When clouds begin to gather, the collection of water enters the air and returns to Earth as rain drops."},
        {"hail", "Just like snow, is when the weather begins to cool while raining. Hail is a form of ice."},
        {"hell", "He plain of existence believed to be where all evil resides once dead."},
        {"heaven", "A place where some believe the good and righteous goes once dead."},
        {"bible", "The book that follows the teachings of God."},
        {"starving", "A state of hunger."},
        {"starvation", "The act of dying from lack of food. Usually caused by a famine."},
        {"vote", "The democratic act of selecting a leader or upon a bill. Usually, it's an illusion."},
        {"voting", "The mindless act of selecting a leader in a wide spread lie of illusion."},
        {"starfish", "A five limb-ed creature, that moves alone the beaches and ocean floor."},
        {"book", "A collection of words that make up a wealth of knowledge."},
        {"look", "The word is used by one to another, to draw attention to a detail."},
        {"took", "A word used in pretence, on someone taking an object, person, etc etc etc."},
        {"gloves", "An item used to protect ones hands from the elements or by protecting one's hands."},
        {"nickel", "An alloy that is mined out of the Earth. Used in everything from currency, bullets, and everyday items."},
        {"dime", "A dime is a ten cent piece of currency."},
        {"penny", "Made of copper, is an old form of currency. Still in circulation, though Canada discontinued it."},
        {"dollar", "Once a considered a lot of money, now has little to no value."},
        {"wreck", "Either meaning someone destroyed something or a type of vehicle used to tow vehicles that have broken down."},
        {"wrecker", "A term used for a tow truck."},
        {"novel", "A fictional book."},
        {"novelty", "Something that is just a hook, a cliched item, or a fad."},
        {"creme", "A manufactured by product of a dessert topping or a topical ointment."},
        {"ice", "The result of water being subjected to the freezing point."},
        {"booze", "A another word for alcohol."},
        {"bozo", "A term used to describe someone behaving like a clown."},
        {"ring", "A single item used to represent a circular design. Can be made out of any item."},
        {"island", "A piece of land cutoff from a continent and surrounded by water."},
        {"locket", "A small symbol of one's love for another. Usually accompanied with a photo."},
        {"lock", "An item or bit of source code that is used to keep those who aren't it's owner out."},
        {"easy", "A low difficulty towards something."},
        {"hard", "Usually the max difficulty selected in a game."},
        {"difficulty", "A word used to describe a certain task."},
        {"axel", "What is called to which the wheels are connected to a vehicle of some sort."},
        {"wheel", "One of the greatest inventions made by pre-bronze era man."},
        {"tire", "Made out of rubber, either solid or inflated. Depending on the make and what it's used for."},
        {"barrel", "Either made out of wood with metal rings, keeping it together or molded out of plastic. Is used to contain liquids."},
        {"borrow", "The act of lending an item to another person."},
        {"lawyer", "One that works for an individual in fighting for them or defending them. With a high knowledge of the law."},
        {"judge", "One that is voted into position and finally decides of a trials outcome."},
        {"testament", "One's account of a scenario that took place."},
        {"testify", "The act of going under oath and giving one's account of events that they had witnessed."},
        {"communism", "An idea created by Karl Marx and has been proven to be a failure."},
        {"fascism", "A political theory which has been proven to not work."},
        {"fascist", "A supporter of the idea of fascism."},
        {"golf", "The sport that has grown into popularity with the elderly and those who feel as if they're important."},
        {"golfer", "The name of someone who plays golf."},
        {"basket", "An item to hold whatever it's owner wishes to carry."},
        {"basket ball", "The sport invented by a Canadian professor."},
        {"present", "This moment in time and space."},
        {"past", "Every moment we step forward, we are finding ourselves slipping away from the past."},
        {"future", "What it beholds is anyone's guess."},
        {"Passover", "A Jewish holiday."},
        {"pass", "When one succeeds in passing a test or task."},
        {"bystander", "One who is not taking place in an event and just watch's from a distance."},
        {"ferry", "A boat used to transport people form one side of land to another across a large body of water."},
        {"fence", "An item made to divide land and keep people and animals out."},
        {"Hanferd", "An unknown person of origin."},
        {"botch", "Too screw something up."},
        {"bitch", "A term for a female dog."},
        {"bull", "A name for a male cow."},
        {"roaster", "Something that roasts meet or vegetables."},
        {"roast", "A piece of beef either backed or boiled."},
        {"espresso", "The name for a strong shot of coffee."},
        {"magazine", "Varying from any subject with articles and stories."},
        {"cannon", "A weapon used during war, which uses gun powder to fire a large shell."},
        {"trigger", "The part of a gun that fires a round of ammo."},
        {"sing", "To use one's voice to either perform a song or too sing along."},
        {"singer", "Someone who sings to music."},
        {"rhythm", "Using a selection of words that sound a like."},
        {"loser", "Someone who either lost at an event or is used as an insult."},
        {"loss", "To lose an item by either having it stolen or by miss placing it."},
        {"lose", "A word used to either describe someone losing at a sport or election or etc."},
        {"win", "When one wins an event or can be used for describing a good day."},
        {"winner", "The one who's crowned victorious over the rest."},
        {"blizzard", "A type of storm usually with high winds and snow."},
        {"root", "The history of someone or can also mean the life limbs of a plant or tree."},
        {"minnow", "Is a word used for a small fresh water or salt water fish."},
        {"miner", "A name given to one who mines for minerals."},
        {"pick", "A word used to give someone an option."},
        {"ore", "A rock with a sample of minerals located in it."},
        {"vampire", "A fictional creature that drinks blood from human hosts."},
        {"leach", "A parasite that sticks to a host, task."},
        {"leap", "Referring to a small jump, most likely over a puddle of water."},
        {"leper", "a person suffering from leprosy."},
        {"disease", "One who has an infection that is slowly either killing them or making them sick."},
        {"cancer", "A disease that slowly and painfully kills it's victim."},
        {"aide", "An assistant."},
        {"add", "A term used to adding something to a situation."},
        {"subtract", "A term to remove something from a situation or doing a math equation."},
        {"aids", "A disease that was constructed and spread by the Government."},
        {"dock", "a wooden path over a body of water leading to a boat."},
        {"box", "An item used to store items and materials."},
        {"laundry", "A collection of dirty laundry."},
        {"duck", "A winged bird or a term used to lower to the ground."},
        {"bill", "A notice to pay a debt."},
        {"lotus", "A flower and the name of a program that was once popular."},
        {"Linux", "An open source operating system."},
        {"complain", "When one vents how a situation isn't working out for them."},
        {"complex", "A word meaning for a difficult question or scenario to solve."},
        {"Lincoln", "The name of a automobile brand and the last name of the sixteenth President of the United States."},
        {"soup", "A mixture of vegetables, meat, and broth. Many variations and served either hot or cold."},
        {"strike", "A term used in Baseball to describe missing a pitch or a collection or workers who wish for better wages."},
        {"union", "A vast collection of people who are members who's job it is to protect it's workers rights."},
        {"traffic", "A congested street with many vehicles, leading to high stress."},
        {"debut", "The release of something to the public."},
        {"scared", "The feeling of fear that's brought on by a situation that's out of one's control."},
        {"help", "With what would you like help with?"},
        {"specs", "8-bit, Text-based user interface (TUI), Two color screen display."},
        {"about", "Jesse Artificial Intelligence/J-A-I Version 1.75\n         Copyright (c)2018 David Taylor GPL 3.0 or Later\n."}



    };
}




int main() {

    uint8_t RandomNumber;
    uint32_t UsersNumber;


    std::size_t i;
    std::string ConvertedInput;
    COORD CursorCoords = {0, 0};

    srand (time(NULL));

    SetPalette(Jesse::Palette1);

    csbiEX.cbSize = sizeof(csbiEX);
	GetConsoleScreenBufferInfoEx(hstdout, &csbiEX);
	csbiEX.dwSize = Jesse::NewScreenBufferDimensions;
	csbiEX.dwCursorPosition = Jesse::CursorCoords;
	csbiEX.wAttributes = Jesse::PrimaryWinColor[Jesse::CurrentWinColor];
	csbiEX.srWindow = Jesse::NewsrWindow;
	csbiEX.dwMaximumWindowSize = Jesse::MaximumWindowSize;
	SetConsoleScreenBufferInfoEx(hstdout, &csbiEX);

    SetConsoleTitle(Jesse::ConsoleTitle);

    JesseLog.open ("JesseLog.txt");

    Color(12);
    std::cout << Jesse::TitleBlock << "\n";
    JesseLog << Jesse::TitleBlock << "\n";

    do {
        Color(8);
        std::cout << "\n" << Jesse::UserSalutations;
        JesseLog << "\n" << Jesse::UserSalutations;

        ClearConsoleToEndOfLine();

        Color(14);
        std::getline (std::cin, Jesse::UserInput);
        JesseLog << Jesse::UserInput;

        std::cout << "\n";
        JesseLog << "\n";

        std::transform(Jesse::UserInput.begin(), Jesse::UserInput.end(), Jesse::UserInput.begin(), ::tolower);

        Jesse::UserInput = RemovePunctuation(Jesse::UserInput);

        if (Jesse::UserInput == "color1")
        {
            Jesse::CurrentColorPalette = Jesse::Palette1;

            SetPalette(Jesse::Palette1);
            CursorCoords.X = csbiEX.dwCursorPosition.X;
            CursorCoords.Y = csbiEX.dwCursorPosition.Y - 3;
            SetConsoleCursorPosition(hstdout, CursorCoords );
       }
        else if (Jesse::UserInput == "color2")
        {
            Jesse::CurrentColorPalette = Jesse::Palette2;

            SetPalette(Jesse::Palette2);
            CursorCoords.X = csbiEX.dwCursorPosition.X;
            CursorCoords.Y = csbiEX.dwCursorPosition.Y - 3;
            SetConsoleCursorPosition(hstdout, CursorCoords );
        }
        else if (Jesse::UserInput == "color3")
        {
            Jesse::CurrentColorPalette = Jesse::Palette3;

            SetPalette(Jesse::Palette3);
            CursorCoords.X = csbiEX.dwCursorPosition.X;
            CursorCoords.Y = csbiEX.dwCursorPosition.Y - 3;
            SetConsoleCursorPosition(hstdout, CursorCoords );
        }
        else if (Jesse::UserInput == "guess")
        {
            do {

                std::cout << "Pick a number from 1 to 10\n\n";
                RandomNumber = rand() % 10 + 1;

                GetInput:

                std::getline (std::cin, Jesse::UserInput);

                std::transform(Jesse::UserInput.begin(), Jesse::UserInput.end(), Jesse::UserInput.begin(), ::tolower);

                Jesse::UserInput = RemovePunctuation(Jesse::UserInput);

                if ((Jesse::UserInput == "quit") || (Jesse::UserInput == "end"))
                {
                    std::cout << "\n\nThank you for playing with ole Jesse.\n\n";
                    break;
                }

                try {
                    UsersNumber = std::stoul(Jesse::UserInput, nullptr, 10);
                }
                catch (const std::invalid_argument& ia)
                {
                    std::cout << "Invalid input, please try again {error: " << ia.what() << "}.\n";
                    goto GetInput;
                }

                if (RandomNumber == UsersNumber)
                {
                    std::cout << "You got the number!\n\nWould you like to play again?";
                    std::getline (std::cin, Jesse::UserInput);

                    if (Jesse::UserInput == "no")
                    {
                        std::cout << "\n\nThank you for playing with ole Jesse.\n\n";
                        break;
                    }
                }
                else
                {
                    std::cout << "That is not the correct number. Please try again\n\n";
                    goto GetInput;
                }
            } while (RandomNumber != -1);

            Jesse::UserInput = "";

        }
        else if (Jesse::UserInput == "randomize")
        {
            RandomizeConsoleBackground();

            CursorCoords.X = csbiEX.dwCursorPosition.X;
            CursorCoords.Y = csbiEX.dwCursorPosition.Y - 3;
            SetConsoleCursorPosition(hstdout, CursorCoords );
        }
        else if (Jesse::UserInput == "funky")
        {
            FunkyConsoleBackground();

            Color(12);
            std::cout << Jesse::TitleBlock << "\n";
            JesseLog << Jesse::TitleBlock << "\n";
        }
        else if (Jesse::UserInput == "cls")
        {
            ClearScreen();
            Color(12);
            std::cout << Jesse::TitleBlock << "\n";
            JesseLog << Jesse::TitleBlock << "\n";
        }
        else
        {
            for(i = 0; i < std::size(Jesse::UserInputAndResponses); i++)
            {
                ConvertedInput = Jesse::UserInputAndResponses[i].PreDefInput;
                std::transform(ConvertedInput.begin(), ConvertedInput.end(), ConvertedInput.begin(), ::tolower);

                ConvertedInput = RemovePunctuation(ConvertedInput);

                if (Jesse::UserInput == ConvertedInput)
                {
                    Color(8);
                    std::cout << Jesse::JesseSalutations;
                    Color(14);
                    std::cout << Jesse::UserInputAndResponses[i].Response << "\n\n";
                    JesseLog << Jesse::JesseSalutations << Jesse::UserInputAndResponses[i].Response << "\n\n";
                    break;
                }
            }

            if (i >= std::size(Jesse::UserInputAndResponses))
            {
                if (Jesse::UserInput == "")
                {
                    Color(8);
                    std::cout << Jesse::JesseSalutations;
                    Color(14);
                    std::cout << "Please do not be shy dear. Speak to ole Jesse.\n\n";
                }
                else
                {
                    if ((Jesse::UserInput == "quit") || (Jesse::UserInput == "end") || (Jesse::UserInput == "exit"))
                    {
                        std::cout << "\n\n";
                        JesseLog << "\n\n";
                    }
                    else
                    {
                    Color(8);
                    std::cout << Jesse::JesseSalutations;
                    Color(14);
                    std::cout << Jesse::IDontUnderstandResponse << "\n\n";
                    JesseLog << Jesse::JesseSalutations << Jesse::IDontUnderstandResponse << "\n\n";
                    }
                }
            }

            Color(6);
            std::cout << Jesse::BlockSeparator1;
            JesseLog << Jesse::BlockSeparator1;

        }

    } while ((Jesse::UserInput != "quit") && (Jesse::UserInput != "end") && (Jesse::UserInput != "exit"));

    Color(15);
    std::cout << Jesse::EndMessage;
    JesseLog << Jesse::EndMessage;
    std::getline (std::cin, Jesse::UserInput);
    JesseLog.close();
    return 0;
}

void Color(UINT8 Foreground)
{
    SetConsoleTextAttribute(hstdout, (Jesse::PrimaryWinColor[Jesse::CurrentWinColor] & 0xF0) | (Foreground & 0x0F));
}


void Color(UINT8 Foreground, UINT8 Background)
{
    SetConsoleTextAttribute(hstdout, ((Foreground & 0x0F) << 4) | (Background & 0x0F));
}


void ClearScreen()
{
    DWORD cellCount;
    DWORD counter;

    SetConsoleTextAttribute(hstdout, Jesse::PrimaryWinColor[Jesse::CurrentWinColor]);
    cellCount = csbiEX.dwSize.X * csbiEX.dwSize.Y;

    FillConsoleOutputAttribute(hstdout, Jesse::PrimaryWinColor[Jesse::CurrentWinColor], cellCount, Jesse::CursorCoords, &counter);
    FillConsoleOutputCharacter(hstdout, (TCHAR) ' ', cellCount, Jesse::CursorCoords, &counter);
    SetConsoleCursorPosition(hstdout, Jesse::CursorCoords );
}


void SetPalette(UINT8 PaletteNum)
{
    std::size_t i;

    csbiEX.cbSize = sizeof(csbiEX);
	GetConsoleScreenBufferInfoEx(hstdout, &csbiEX);

    for(i = 0; i < 16; i++)
    {
        csbiEX.ColorTable[i] = Jesse::ColorTable[PaletteNum][i];
    }

  	csbiEX.dwSize = Jesse::NewScreenBufferDimensions;
	csbiEX.srWindow = Jesse::NewsrWindow;
	csbiEX.dwMaximumWindowSize = Jesse::MaximumWindowSize;

    SetConsoleScreenBufferInfoEx(hstdout, &csbiEX);
}


void RandomizeConsoleBackground()
{
    UINT8 Color;
    DWORD counter;
    DWORD cellCount;
    COORD CurrentCursorCoords = {0, 0};
    std::size_t i;

    csbiEX.cbSize = sizeof(csbiEX);
    GetConsoleScreenBufferInfoEx(hstdout, &csbiEX);

    cellCount = csbiEX.dwSize.X * csbiEX.dwSize.Y;

    srand(time(NULL));

    for(i = 0; i < cellCount; i++)
    {
        Color = ((rand() % 4 + 2) << 4) + 14;
        CurrentCursorCoords.X = i % csbiEX.dwSize.X;
        CurrentCursorCoords.Y = i / csbiEX.dwSize.X;

        FillConsoleOutputAttribute(hstdout, Color, 1, CurrentCursorCoords, &counter);
    }
}

void FunkyConsoleBackground()
{
    UINT8 Color;
    DWORD counter;
    DWORD cellCount;
    COORD CurrentCursorCoords = {0, 0};
    std::size_t CountIterater, FunkyTimes;

    ClearScreen();

    csbiEX.cbSize = sizeof(csbiEX);
    GetConsoleScreenBufferInfoEx(hstdout, &csbiEX);

    cellCount = csbiEX.dwSize.X * Jesse::MaximumWindowSize.Y;

    srand(time(NULL));

    for(FunkyTimes = 0; FunkyTimes < 50; FunkyTimes++)
    {
        SetPalette(rand() % 3);
        for(CountIterater = 0; CountIterater < cellCount; CountIterater++)
        {
            Color = ((rand() % 4 + 2) << 4) + 14;
            CurrentCursorCoords.X = CountIterater % csbiEX.dwSize.X;
            CurrentCursorCoords.Y = CountIterater / csbiEX.dwSize.X;

            FillConsoleOutputAttribute(hstdout, Color, 1, CurrentCursorCoords, &counter);

        }
    }

    ClearScreen();
    SetPalette(Jesse::CurrentColorPalette);

}



void ClearConsoleToEndOfLine()
{
    DWORD counter;
    DWORD cellCount;
    COORD CursorCoords;

    csbiEX.cbSize = sizeof(csbiEX);
    GetConsoleScreenBufferInfoEx(hstdout, &csbiEX);

    CursorCoords.X = csbiEX.dwCursorPosition.X;
    CursorCoords.Y = csbiEX.dwCursorPosition.Y;

    cellCount = csbiEX.dwMaximumWindowSize.X - CursorCoords.X;
    FillConsoleOutputCharacter(hstdout, (TCHAR) ' ', cellCount, CursorCoords, &counter);

    SetConsoleCursorPosition(hstdout, CursorCoords );
}
std::string RemovePunctuation (std::string& Message)
{
    uint64_t MsgPos;

    for (MsgPos = 0; MsgPos <= Message.size(); MsgPos++) { if(ispunct(Message[MsgPos])) Message.erase(MsgPos--, 1); }

    return Message;
}

//October 22, 2017 Qbasic Coded - December 28, 2018 C++ Version Coded
//Two dates that will remain close to my heart.
