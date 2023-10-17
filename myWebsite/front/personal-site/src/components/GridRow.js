

export default function ImageRow() {
    return (
        <div id="image-row">
            <RecipeLink photoUrl={"pancake.png"} imageName={"Recipe 1"} />
        </div>
    );
}

function RecipeLink( {photoUrl, imageName} ) {
    return (
        <>
        <div><img className="image-list-image" src={require(`../assets/img/${photoUrl}`)} alt={`${imageName}`}/></div>
        <div><h3>{imageName}</h3></div>
        </>
    ); 
};

