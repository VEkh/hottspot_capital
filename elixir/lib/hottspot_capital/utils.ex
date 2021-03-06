defmodule HottspotCapital.Utils do
  def previous_weekdate(current_date) do
    current_date
    |> Date.range(Date.add(current_date, -7))
    |> Enum.filter(fn date ->
      Date.day_of_week(date) in 1..5 && date != current_date
    end)
    |> List.first()
  end

  def price_movement(from: from, to: to) do
    ((to - from) / from)
    |> Float.round(6)
  end

  def update_in_and_build(map, list, update_path \\ [])
  def update_in_and_build(map, [], _update_path), do: map
  def update_in_and_build(map, [_ | []], _update_path), do: map

  def update_in_and_build(map, [head | [tail | []]], update_path) do
    update_path = update_path ++ [head]
    put_in(map, update_path, tail)
  end

  def update_in_and_build(map, [head | tail], update_path) do
    update_path = update_path ++ [head]

    map =
      update_in(map, update_path, fn
        %{} = old_value -> old_value
        _ -> %{}
      end)

    update_in_and_build(map, tail, update_path)
  end
end
