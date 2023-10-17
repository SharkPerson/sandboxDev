import { useState } from 'react';

let initialRecipes = [
    { id: 0, title: "Hello World" }
]

export default function Arrary() {
    const [recipes, setRecipes] = useState(initialRecipes);

    return (
        <>
        <h1>My Recipes :)</h1>
        <div> { recipes.map(
            recipe => {
                return (
                    <div>
                    <p> {recipe.title} </p>
                    </div>
                )
            })
        });
        </div>
        </>
);
}
