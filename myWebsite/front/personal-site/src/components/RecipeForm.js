
export default function CreateRecipeForm() {
    const status = 'success';
    if (status === 'success') {
        return (
            <>
            <div>
            <InputSection />
            </div>
            <div>
            <RequestRecipes />
            </div>
            </>
        );
    };
};

function InputSection() {
    return (
        <>
        <div>
        <h2>Please input a new recipe name</h2>
        <form>
            <input type="text" />
        </form>
        <button type="submit">Create</button>
        </div>
        </>
    );
};

function RequestRecipes() {
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("demo").innerHTML = this.responseText;
        }
    };

    // 2nd arg is a file on a server
    // 3rd arg is true or false for async
    xhttp.open("GET", "test.txt", true);
    xhttp.send();
}
