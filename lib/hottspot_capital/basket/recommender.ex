defmodule HottspotCapital.Basket.Recommender do
  alias HottspotCapital.Basket.MovementCalculator
  alias HottspotCapital.Company

  def recommend() do
    Company.get_largest(200)
    |> Task.async_stream(fn %{symbol: symbol} ->
      IO.puts("Calculating movement for: #{symbol}")
      MovementCalculator.calculate(symbol)
    end)
    |> Enum.map(&Kernel.elem(&1, 1))
    |> filter_ordered_movements()
    |> Enum.sort(fn a, b ->
      [a_last_close, b_last_close] =
        [a, b]
        |> Enum.map(fn movement ->
          get_in(
            movement,
            ["reference", "last_two_closes", Access.at(0), "close"]
          )
        end)

      a_last_close <= b_last_close
    end)
  end

  defp filter_ordered_movements(movements) do
    movements
    |> Enum.filter(fn movement ->
      %{
        "basket_movement" => basket_movement,
        "reference" => %{"movement" => reference_movement}
      } = movement

      basket_movement >= 0.1 && reference_movement <= 0
    end)
  end
end
