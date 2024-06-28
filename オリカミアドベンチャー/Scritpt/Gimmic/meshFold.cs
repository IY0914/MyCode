using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class meshFold : MonoBehaviour
{
    MeshFilter meshFilter;
    Vector2 pos;
    public Cursor cursor;


    Vector3[] myVertices;
    Vector3[] myNormals;

    // Start is called before the first frame update
    void Start()
    {
        meshFilter = this.gameObject.GetComponent<MeshFilter>();

        Mesh mesh = meshFilter.mesh;

        myVertices = mesh.vertices;
        myNormals = mesh.normals;

        myVertices[2].y = myVertices[2].y - Mathf.Cos(Time.deltaTime);
        myVertices[3].y = myVertices[3].y - Mathf.Cos(Time.deltaTime);


        Debug.Log(myVertices.Length);

    }

    // Update is called once per frame
    void Update()
    {

        myVertices[2].y = myVertices[2].y - Mathf.Cos(Time.deltaTime);
        myVertices[3].y = myVertices[3].y - Mathf.Cos(Time.deltaTime);
        for (int i = 0; i < myVertices.Length; i++)
        {
           
        }
        
        meshFilter.mesh.SetVertices(myVertices);
        
        
    }
}
