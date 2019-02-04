open Jest;

open Expect;

open Tennis;

describe("Test tooling functions", () => {
  test("Given PlayerOne when other player is PlayerTwo", () =>
    expect(other(PlayerOne)) |> toEqual(PlayerTwo)
  );
  test("Given PlayerOne string_of_player is player 1", () =>
    expect(string_of_player(PlayerOne)) |> toEqual("Player 1")
  );
  test("Given PlayerTwo string_of_player is player 2", () =>
    expect(string_of_player(PlayerTwo)) |> toEqual("Player 2")
  );
  test("Given Love/Love string_of_score is 0 - 0", () => {
    let s = Points({playerOne: Love, playerTwo: Love});
    expect(string_of_score(s)) |> toEqual("Player 1 : 0 - 0 : Player 2");
  });
  test("Given Thirty/Fifteen string_of_score is 30 - 15", () => {
    let s = Points({playerOne: Thirty, playerTwo: Fifteen});
    expect(string_of_score(s)) |> toEqual("Player 1 : 30 - 15 : Player 2");
  });
  test("Given Deuce string_of_score is 40A", () => {
    let s = Deuce;
    expect(string_of_score(s)) |> toEqual("Player 1 : 40A : Player 2");
  });
  test(
    "Given Forty of (PlayerOne, Fifteen) PlayerTwo string_of_score is 40 - 15",
    () => {
    let s = Forty({player: PlayerOne, otherPlayerPoint: Fifteen});
    expect(string_of_score(s)) |> toEqual("Player 1 : 40 - 15 : Player 2");
  });
  test(
    "Given Forty of (PlayerTwo, Thirty) PlayerTwo string_of_score is 40 - 15",
    () => {
    let s = Forty({player: PlayerTwo, otherPlayerPoint: Thirty});
    expect(string_of_score(s)) |> toEqual("Player 1 : 30 - 40 : Player 2");
  });
  test("Given Advantage PlayerTwo string_of_score is - Advantage", () => {
    let s = Advantage(PlayerTwo);
    expect(string_of_score(s)) |> toEqual("Player 1 :  - Advantage : Player 2");
  });
});

describe("Test tennis transitions", () => {
  test("Given deuce when player wins then score is correct", () =>
    expect(scoreWhenDeuce(PlayerOne)) |> toEqual(Advantage(PlayerOne))
  );
  test("Given advantage when advantaged player wins then score is correct", () => {
    let advantagedPlayer = PlayerOne;
    let winner = advantagedPlayer;
    expect(Tennis.scoreWhenAdvantage(advantagedPlayer, winner))
    |> toEqual(Game(advantagedPlayer));
  });
  test("Given advantage when the other player wins then score is Deuce", () => {
    let advantagedPlayer = PlayerOne;
    let winner = other(advantagedPlayer);
    expect(Tennis.scoreWhenAdvantage(advantagedPlayer, winner))
    |> toEqual(Deuce);
  });
  test("Given player: 40 when wins then score is Game for this player", () => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect(scoreWhenForty(fortyThirty, fortyThirty.player))
    |> toEqual(Game(fortyThirty.player));
  });
  test("Given player: 40 | other : 30 when other wins then score is Deuce", () => {
    let fortyThirty = {player: PlayerOne, otherPlayerPoint: Thirty};
    expect(scoreWhenForty(fortyThirty, other(fortyThirty.player)))
    |> toEqual(Deuce);
  });
  test("Given player: 40 | other 0 when other wins then score is 40/15", () => {
    let fortyLove = {player: PlayerOne, otherPlayerPoint: Love};
    let fortyFifteen = {player: PlayerOne, otherPlayerPoint: Fifteen};
    expect(scoreWhenForty(fortyLove, other(fortyLove.player)))
    |> toEqual(Forty(fortyFifteen));
  });
  test(
    "Given player: 15 | other : 15 when player wins then score is 30/15", () => {
    let fifteenFifteen = {playerOne: Fifteen, playerTwo: Fifteen};
    let thirtyFifteen = {playerOne: Thirty, playerTwo: Fifteen};
    expect(scoreWhenPoints(fifteenFifteen, PlayerOne))
    |> toEqual(Points(thirtyFifteen));
  });
  test("Given player: 0 | other : 15 when other wins then score is 0/30", () => {
    let loveFifteen = {playerOne: Love, playerTwo: Fifteen};
    let loveThirty = {playerOne: Love, playerTwo: Thirty};
    expect(scoreWhenPoints(loveFifteen, other(PlayerOne)))
    |> toEqual(Points(loveThirty));
  });
  test(
    "Given player: 30 | other : 15 when player wins then score is 40/15", () => {
    let thirtyFifteen = {playerOne: Thirty, playerTwo: Fifteen};
    let fortyFifteen = {player: PlayerOne, otherPlayerPoint: Fifteen};
    expect(scoreWhenPoints(thirtyFifteen, PlayerOne))
    |> toEqual(Forty(fortyFifteen));
  });
});