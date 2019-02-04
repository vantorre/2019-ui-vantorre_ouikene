/* Types */
type player =
  | PlayerOne
  | PlayerTwo;

type point =
  | Love
  | Fifteen
  | Thirty;

type pointsData = {
  playerOne: point,
  playerTwo: point
};

type fortyData = {
  player, /* The player who have forty points */
  otherPlayerPoint: point
};

type score =
  | Points(pointsData)
  | Forty(fortyData)
  | Deuce
  | Advantage(player)
  | Game(player);

/* Type transformation functions */
let string_of_player = player =>
  switch player {
  | PlayerOne => "Player 1"
  | PlayerTwo => "Player 2"
  };

let string_of_point = point =>
  switch point {
  | Love => "0"
  | Fifteen => "15"
  | Thirty => "30"
  };

let string_of_score = current =>
  string_of_player(PlayerOne)
  ++ " : "
  ++ (
    switch current {
    | Points(p) =>
      string_of_point(p.playerOne) ++ " - " ++ string_of_point(p.playerTwo)
    | Deuce => "40A"
    | Game(g) => g == PlayerOne ? "Game - " : " - Game"
    | Advantage(a) => a == PlayerOne ? "Advantage - " : " - Advantage"
    | Forty(f) =>
      f.player == PlayerOne ?
        "40 - " ++ string_of_point(f.otherPlayerPoint) :
        string_of_point(f.otherPlayerPoint) ++ " - 40"
    }
  )
  ++ " : "
  ++ string_of_player(PlayerTwo);

/* Tooling functions */
let other = player =>
  switch player {
  | PlayerOne => PlayerTwo
  | PlayerTwo => PlayerOne
  };

let incrementPoint: point => option(point) =
  point =>
    switch point {
    | Love => Some(Fifteen)
    | Fifteen => Some(Thirty)
    | Thirty => None
    };

let pointTo = (player, point, current) =>
  switch player {
  | PlayerOne => {...current, playerOne: point}
  | PlayerTwo => {...current, playerTwo: point}
  };

let pointFor = (player, current) =>
  switch player {
  | PlayerOne => current.playerOne
  | PlayerTwo => current.playerTwo
  };

/* Score management functions */
let scoreWhenDeuce: player => score = winner => Advantage(winner);

let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) =>
    advantagedPlayer == winner ? Game(winner) : Deuce;

let scoreWhenForty = (current, winner) =>
  current.player == winner ?
    Game(winner) :
    (
      switch (incrementPoint(current.otherPlayerPoint)) {
      | Some(p) => Forty({...current, otherPlayerPoint: p})
      | None => Deuce
      }
    );

let scoreWhenGame: player => score = winner => Game(winner);

let scoreWhenPoints = (current, winner) =>
  switch (current |> pointFor(winner) |> incrementPoint) {
  | Some(np) => Points(pointTo(winner, np, current))
  | None =>
    Forty({
      player: winner,
      otherPlayerPoint: current |> pointFor(other(winner))
    })
  };

let score = (current, winner) =>
  switch current {
  | Points(p) => scoreWhenPoints(p, winner)
  | Forty(f) => scoreWhenForty(f, winner)
  | Deuce => scoreWhenDeuce(winner)
  | Advantage(a) => scoreWhenAdvantage(a, winner)
  | Game(g) => scoreWhenGame(g)
  };

let newGame = Points({playerOne: Love, playerTwo: Love});